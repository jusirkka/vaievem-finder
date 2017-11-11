#include "timetable.h"
#include "database.h"
#include <QDebug>
#include <cmath>
#include <algorithm>
#include <QDateTime>
#include <QTimeZone>

Timetable::~Timetable() {}


Timetable::Timetable()
    : QAbstractListModel()
{
}



int Timetable::rowCount(const QModelIndex&) const {
    return mSchedules.size();
}


QVariant Timetable::data(const QModelIndex &index, int role) const {


    if (!index.isValid()) {
        return QVariant();
    }

    Schedule s = mSchedules[index.row()];

    if (role == LineNameRole) {
        return mLineNames[s.line];
    }

    if (role == LineColorRole) {
        return mLineColors[s.line];
    }

    if (role == OriginRole) {
        return mStopNames[s.origin];
    }

    if (role == DestinationRole) {
        return mStopNames[s.destination];
    }

    if (role == LeavingRole) {
        return s.leaving;
    }

    if (role == ArrivingRole) {
        return s.arriving;
    }

    return QVariant();
}



Timetable::RoleHash Timetable::roleNames() const {
    RoleHash roles;
    roles[LineNameRole] = "line";
    roles[LineColorRole] = "color";
    roles[OriginRole] = "origin";
    roles[DestinationRole] = "destination";
    roles[LeavingRole] = "leaving";
    roles[ArrivingRole] = "arriving";
    return roles;
}

static float distance(const QGeoCoordinate& p1, const QGeoCoordinate& p2) {
    static float c = ::cos(37.13 * 3.14159 / 180);
    static float k = 1000. / 9.;
    float dx = c * (p2.longitude() - p1.longitude());
    float dy = p2.latitude() - p1.latitude();
    return k * ::sqrt(dx*dx + dy*dy);
}

class Stop {
public:
    Timetable::IdList lines;
    float dist;
};

void Timetable::find(const QGeoCoordinate& origin, const QGeoCoordinate& destination) {
    mLineNames.clear();
    mLineIds.clear();
    mLineColors.clear();
    mStops.clear();
    mStopNames.clear();
    mStopLocations.clear();
    mSchedules.clear();
    QString sql = "select c.line_id, c.stop_id, s.latitude, s.longitude "
                  "from schedule c "
                  "join stop s "
                  "on c.stop_id=s.id "
                  "where c.connection_id=1 "
                  "order by c.line_id, c.hours, c.minutes";

    Database::RowVector rows = Database::Query(sql);
    // 1. find stops that are within 1.0 kms from the origin
    QMap<int, Stop> accepted;
    IdList rejected;
    for (const Database::Row& row: rows) {
        auto line_id = row[0].toInt();
        auto stop_id = row[1].toInt();

        if (!mStops.contains(line_id)) mStops[line_id] = IdList();
        mStops[line_id].append(stop_id);

        if (rejected.contains(stop_id)) continue; // already rejected
        if (accepted.contains(stop_id)) { // already accepted
            accepted[stop_id].lines.append(line_id);
            continue;
        }

        // unhandled stop
        QGeoCoordinate p(row[2].value<float>(), row[3].value<float>());
        mStopLocations[stop_id] = p;

        float dist = distance(origin, p);
        if (dist < 1.0) {
            Stop s;
            s.lines.append(line_id);
            s.dist = dist;
            accepted[stop_id] = s;
        } else {
            rejected.append(stop_id);
        }
    }
    // 2. sort & filter them by distance so that each stop belongs to different lines (e.g. (1,2), (3), (4, 5))
    IdList astops = accepted.keys();
    std::sort(astops.begin(), astops.end(), [&accepted](int a, int b) {
        return accepted[a].dist < accepted[b].dist;
    });
    IdList alines;
    for (auto key: astops) {
        IdList lines;
        for (auto line: accepted[key].lines) {
            if (alines.contains(line)) continue;
            lines.append(line);
            alines.append(line);
        }
        if (!lines.isEmpty()) {
            accepted[key].lines = lines;
        } else {
            accepted.remove(key);
        }
    }
    // clean up stops
    for (auto line: mStops.keys()) {
        if (!alines.contains(line)) {
            mStops.remove(line);
        }
    }
    QMapIterator<int, Stop> iter(accepted);
    while (iter.hasNext()) {
        iter.next();
        Stop s = iter.value();
        int first_stop = iter.key();
        for (auto line: s.lines) {
            while (!mStops[line].isEmpty() && (mStops[line].first() != first_stop)) {
                mStops[line].takeFirst();
            }
            if (mStops[line].length() < 2) {
                mStops.remove(line);
            }
        }
    }
    // 3. from the stops of these lines, find stops that are within 1.0 kms from the destination
    // 4. for each line find the closest stop to the destination.
    for (auto line: mStops.keys()) {
        int closest = mStops[line].first();
        float min_dist = distance(destination, mStopLocations[closest]);
        for (auto stop_id: mStops[line]) {
            float dist = distance(destination, mStopLocations[stop_id]);
            if (dist < 1.0 && dist < min_dist) {
                min_dist = dist;
                closest = stop_id;
            }
        }
        if (closest == mStops[line].first()) { // not found
            mStops.remove(line);
        } else { // remove tail
            while (mStops[line].last() != closest) {
                mStops[line].takeLast();
            }
        }
    }
    // qDebug() << mStops;

    if (mStops.isEmpty()) return;

    // fetch data from the database
    sql = "select s1.line_id, s1.stop_id, s1.hours, s1.minutes, s2.stop_id, s2.hours, s2.minutes "
          "from schedule s1 "
          "join schedule s2 "
          "on s1.connection_id=s2.connection_id and s1.line_id=s2.line_id "
          "join weekday w on s1.weekday_set_id=w.set_id "
          "where w.weekday=? "
          "and s1.hours*60 + s1.minutes > ? "
          "and (%1) " // list of "(s1.line_id=? and s1.stop_id=? and s2.stop_id=?)" joined by " or "
          "order by s1.hours, s1.minutes";

    Database::VariableVector v;
    QStringList ph;

    QDateTime dt = QDateTime::currentDateTimeUtc();
    dt.toTimeZone(QTimeZone("Europe/Lisbon"));

    alines.clear(); // collect found lines

    int hours = 0;
    int first = dt.time().hour() * 60 + dt.time().minute();
    QDate day = dt.date();
    while (hours < 12) {
        v.clear();
        ph.clear();

        // qDebug() << day << first;
        v << day.dayOfWeek();
        v << first;
        for (auto line: mStops.keys()) {
            ph << "(s1.line_id=? and s1.stop_id=? and s2.stop_id=?)";
            v << line << mStops[line].first() << mStops[line].last();
        }

        rows = Database::Query(sql.arg(ph.join(" or ")), v);
        qint64 msecs_0 = 0;
        qint64 msecs_1 = 0;
        for (const Database::Row& row: rows) {
            Schedule s;
            s.line = row[0].toInt();
            s.origin = row[1].toInt();
            s.destination = row[4].toInt();
            QDateTime leaving(day, QTime(row[2].toInt(), row[3].toInt()), QTimeZone("Europe/Lisbon"));
            s.leaving = leaving.toMSecsSinceEpoch();
            QDateTime arriving(day, QTime(row[5].toInt(), row[6].toInt()), QTimeZone("Europe/Lisbon"));
            s.arriving = arriving.toMSecsSinceEpoch();

            if (msecs_0 == 0) {
                msecs_0 = s.leaving;
            }
            msecs_1 = s.leaving;

            mSchedules.append(s);

            if (!alines.contains(s.line)) {
                alines.append(s.line);
            }
        }

        day = day.addDays(1);
        first = 0;

        hours += (msecs_1 - msecs_0) / (3600 * 1000);
        // qDebug() << "hours" << hours;
    }


    for (auto line: mStops.keys()) {
        if (!alines.contains(line)) {
            mStops.remove(line);
        }
    }

    IdList stops;
    for (auto line: mStops.keys()) {
        for (auto stop_id: mStops[line]) {
            if (!stops.contains(stop_id)) {
                stops.append(stop_id);
            }
        }
    }
    for (auto stop_id: mStopLocations.keys()) {
        if (!stops.contains(stop_id)) {
            mStopLocations.remove(stop_id);
        }
    }
    stops.clear();
    for (auto line: mStops.keys()) {
        int stop_id = mStops[line].first();
        if (!stops.contains(stop_id)) {
            stops.append(stop_id);
        }
        stop_id = mStops[line].last();
        if (!stops.contains(stop_id)) {
            stops.append(stop_id);
        }
    }
    sql = "select id, name from stop where id in (%1)";
    v.clear();
    ph.clear();
    for (auto stop_id: stops) {
        ph << "?";
        v.append(stop_id);
    }
    rows = Database::Query(sql.arg(ph.join(",")), v);
    for (const Database::Row& row: rows) {
        auto stop_id = row[0].toInt();
        mStopNames[stop_id] = row[1].toString();
    }

    sql = "select id, name, color from line where id in (%1)";
    v.clear();
    ph.clear();
    for (auto line: mStops.keys()) {
        ph << "?";
        v.append(line);
    }
    rows = Database::Query(sql.arg(ph.join(",")), v);
    for (const Database::Row& row: rows) {
        auto line = row[0].toInt();
        auto name = row[1].toString();
        QColor c;
        c.setNamedColor(row[2].toString());
        mLineNames[line] = name;
        mLineIds[name] = line;
        mLineColors[line] = c;
    }
}

QStringList Timetable::lines() const {
    return mLineNames.values();
}

QList<int> Timetable::stops(const QString& line) const {
    return mStops[mLineIds[line]];
}

QColor Timetable::color(const QString& line) const {
    return mLineColors[mLineIds[line]];
}

QString Timetable::name(int id) const {
   return mStopNames[id];
}

QGeoCoordinate Timetable::location(int id) const {
    return mStopLocations[id];
}

