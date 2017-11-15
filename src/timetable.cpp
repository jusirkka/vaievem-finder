#include "timetable.h"
#include "database.h"
#include <QDebug>
#include <cmath>
#include <algorithm>
#include <QDateTime>
#include <QTimeZone>
#include <QSettings>
#include <QGuiApplication>

Timetable::~Timetable() {}


Timetable::Timetable()
    : QAbstractListModel()
    , mWalkingDist(1.0)
{
    bool ok;
    QSettings s(QSettings::IniFormat, QSettings::UserScope, qApp->applicationName(), "settings");
    mWalkingDist = s.value("walking-distance", 1.0).toFloat(&ok);
    if (!ok) {
        mWalkingDist = 1.0;
    }
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
        return mLines[s.line].name;
    }

    if (role == LineColorRole) {
        return mLines[s.line].color;
    }

    if (role == OriginRole) {
        return mStops[s.origin].name;
    }

    if (role == DestinationRole) {
        return mStops[s.destination].name;
    }

    if (role == LeavingRole) {
        return s.leaving;
    }

    if (role == ArrivingRole) {
        return s.arriving;
    }

    if (role == StopsRole) {
        return QVariant::fromValue(mLines[s.line].stops);
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
    roles[StopsRole] = "stops";
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
    beginResetModel();
    mLines.clear();
    mStops.clear();
    mSchedules.clear();
    QString sql = "select c.line_id, c.stop_id, s.latitude, s.longitude, c.season_id, c.weekday_set_id "
                  "from schedule c "
                  "join stop s "
                  "on c.stop_id=s.id "
                  "where c.connection_id=1 "
                  "order by c.line_id, c.hours, c.minutes";

    Database::RowVector rows = Database::Query(sql);
    // 1. find stops that are within mWalkingDist kms from the origin
    QMap<int, Stop> accepted;
    // filter out other seasons & weekdays - assumption is that the stop ordering is identical for these
    // TODO might be easier to define lines independently of schedule
    QMap<int, int> weekdayFilter;
    QMap<int, int> seasonFilter;
    IdList rejected;
    for (const Database::Row& row: rows) {
        auto line_id = row[0].toInt();
        auto stop_id = row[1].toInt();
        auto season_id = row[4].toInt();
        auto weekday_id = row[5].toInt();

        if (!weekdayFilter.contains(line_id)) {
            weekdayFilter[line_id] = weekday_id;
        } else {
            if (weekdayFilter[line_id] != weekday_id) continue;
        }

        if (!seasonFilter.contains(line_id)) {
            seasonFilter[line_id] = season_id;
        } else {
            if (seasonFilter[line_id] != season_id) continue;
        }

        mLines[line_id].stops.append(stop_id);

        if (rejected.contains(stop_id)) continue; // already rejected
        if (accepted.contains(stop_id)) { // already accepted
            accepted[stop_id].lines.append(line_id);
            continue;
        }

        // unhandled stop
        QGeoCoordinate p(row[2].value<float>(), row[3].value<float>());
        mStops[stop_id].coord = p;

        float dist = distance(origin, p);
        if (dist < mWalkingDist) {
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
    // clean up lines
    for (auto line: mLines.keys()) {
        if (!alines.contains(line)) {
            mLines.remove(line);
        }
    }
    QMapIterator<int, Stop> iter(accepted);
    while (iter.hasNext()) {
        iter.next();
        Stop s = iter.value();
        int first_stop = iter.key();
        for (auto line: s.lines) {
            while (!mLines[line].stops.isEmpty() && (mLines[line].stops.first() != first_stop)) {
                mLines[line].stops.takeFirst();
            }
            if (mLines[line].stops.length() < 2) {
                mLines.remove(line);
            }
        }
    }
    // 3. from the stops of these lines, find stops that are within (mWalkingDist - startDist) kms from the destination
    // 4. for each line find the closest stop to the destination.
    for (auto line: mLines.keys()) {
        int closest = mLines[line].stops.first();
        float walkingLeft = mWalkingDist - accepted[closest].dist;
        // qDebug() << "walking left = " << walkingLeft;
        float min_dist = distance(destination, mStops[closest].coord);
        for (auto stop_id: mLines[line].stops) {
            float dist = distance(destination, mStops[stop_id].coord);
            if (dist < walkingLeft && dist < min_dist) {
                // qDebug() << "distance to destination = " << dist;
                min_dist = dist;
                closest = stop_id;
            }
        }
        if (closest == mLines[line].stops.first()) { // not found
            mLines.remove(line);
        } else { // remove tail
            while (mLines[line].stops.last() != closest) {
                mLines[line].stops.takeLast();
            }
        }
    }
    // qDebug() << mLines;

    if (mLines.isEmpty()) {
        endResetModel();
        return;
    }

    // fetch data from the database
    sql = "select s1.line_id, s1.stop_id, s1.hours, s1.minutes, s2.stop_id, s2.hours, s2.minutes "
          "from schedule s1 "
          "join schedule s2 "
          "on s1.connection_id=s2.connection_id and s1.line_id=s2.line_id and s1.weekday_set_id=s2.weekday_set_id and s1.season_id=s2.season_id "
          "join weekday w on s1.weekday_set_id=w.set_id "
          "join season e on s1.season_id=e.id "
          "where w.weekday=? "
          "and ((e.end > e.start and ? >= e.start and ? <= e.end) or (e.end < e.start and (? <= e.end or ? >= e.start))) "
          "and s1.hours*60 + s1.minutes > ? "
          "and (%1) " // list of "(s1.line_id=? and s1.stop_id=? and s2.stop_id=?)" joined by " or "
          "order by s1.hours, s1.minutes";

    Database::VariableVector v;
    QStringList ph;

    QDateTime dt = QDateTime::currentDateTimeUtc();
    dt = dt.toTimeZone(QTimeZone("Europe/Lisbon"));

    alines.clear(); // collect found lines

    int hours = 0;
    int count = 0;
    int first = dt.time().hour() * 60 + dt.time().minute();
    QDate day = dt.date();
    while (hours < 6 && count < 5) {
        v.clear();
        ph.clear();

        // qDebug() << day << first;
        v << day.dayOfWeek();
        int season_id = day.month() * 32 + day.day();
        v << season_id << season_id << season_id << season_id;
        v << first;
        for (auto line: mLines.keys()) {
            ph << "(s1.line_id=? and s1.stop_id=? and s2.stop_id=?)";
            v << line << mLines[line].stops.first() << mLines[line].stops.last();
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
        count += 1;

        hours += (msecs_1 - msecs_0) / (3600 * 1000);
        // qDebug() << "hours" << hours;
//        if (hours == 0) {
//            qDebug() << day;
//            for (auto line: mLines.keys()) {
//                qDebug() << "Line " << line;
//                qDebug() << mLines[line].stops;
//            }
//        }
    }


    for (auto line: mLines.keys()) {
        if (!alines.contains(line)) {
            mLines.remove(line);
        }
    }

    if (mLines.isEmpty()) {
        endResetModel();
        return;
    }

    IdList stops;
    for (auto line: mLines.keys()) {
        for (auto stop_id: mLines[line].stops) {
            if (!stops.contains(stop_id)) {
                stops.append(stop_id);
            }
        }
    }
    for (auto stop_id: mStops.keys()) {
        if (!stops.contains(stop_id)) {
            mStops.remove(stop_id);
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
        mStops[stop_id].name = row[1].toString();
    }

    sql = "select id, name, color from line where id in (%1)";
    v.clear();
    ph.clear();
    for (auto line: mLines.keys()) {
        ph << "?";
        v.append(line);
    }
    rows = Database::Query(sql.arg(ph.join(",")), v);
    for (const Database::Row& row: rows) {
        auto line = row[0].toInt();
        auto name = row[1].toString();
        QColor c;
        c.setNamedColor(row[2].toString());
        mLines[line].name = name;
        mLines[line].color = c;
    }

    endResetModel();
}

QStringList Timetable::lines() const {
    QStringList names;
    for (const LineData& d: mLines.values()) {
        if (!names.contains(d.name)) {
            names << d.name;
        }
    }
    return names;
}

QList<int> Timetable::stops(const QString& line) const {
    for (const LineData& d: mLines.values()) {
        if (d.name == line) {
            return d.stops;
        }
    }
    return IdList();
}

QColor Timetable::color(const QString& line) const {
    for (const LineData& d: mLines.values()) {
        if (d.name == line) {
            return d.color;
        }
    }
    return QColor();
}

QString Timetable::name(int id) const {
   return mStops[id].name;
}

QGeoCoordinate Timetable::location(int id) const {
    return mStops[id].coord;
}


void Timetable::setWalkingDistance(float wd) {
    if (wd != mWalkingDist && wd >= 1.0 && wd <= 5.0) {
        mWalkingDist = wd;
        QSettings s(QSettings::IniFormat, QSettings::UserScope, qApp->applicationName(), "settings");
        s.setValue("walking-distance", mWalkingDist);
        emit walkingDistanceChanged(mWalkingDist);
    }
}

float Timetable::walkingDistance() const {
    return mWalkingDist;
}
