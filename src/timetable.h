#ifndef TIMETABLES_H
#define TIMETABLES_H

#include <QAbstractListModel>
#include <QtPositioning/QGeoCoordinate>
#include <QColor>


class Timetable: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(float walkingDistance
               READ walkingDistance
               WRITE setWalkingDistance
               NOTIFY walkingDistanceChanged)

public:

    enum TimetableRoles {
        LineNameRole = Qt::UserRole + 1,
        LineColorRole,
        OriginRole,
        DestinationRole,
        LeavingRole,
        ArrivingRole,
        StopsRole
    };


public:

    using IdList = QList<int>;
    using RoleHash = QHash<int, QByteArray>;

    Timetable();

    //! Reimplemented from QAbstractItemModel
    int rowCount(const QModelIndex &parent) const override;

    //! Reimplemented from QAbstractItemModel
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //! Reimplemented from QAbstractItemModel
    RoleHash roleNames() const override;

    Q_INVOKABLE void find(const QGeoCoordinate& origin, const QGeoCoordinate& destination);
    Q_INVOKABLE void reset();
    Q_INVOKABLE QStringList lines() const;
    Q_INVOKABLE QList<int> stops(const QString& line) const;
    Q_INVOKABLE QColor color(const QString& line) const;
    Q_INVOKABLE QString name(int id) const;
    Q_INVOKABLE QGeoCoordinate location(int id) const;

    void setWalkingDistance(float);
    float walkingDistance() const;

    ~Timetable();

signals:

    void walkingDistanceChanged(float);

private:

    class Schedule {
    public:
        int line; // NameMap id
        int origin; // NameMap id
        int destination; // NameMap id
        qint64 leaving; // msecs since epoch
        qint64 arriving; // msecs since epoch
    };

    class StopData {
    public:
        QGeoCoordinate coord;
        QString name;
    };

    class LineData {
    public:
        QString name;
        QColor color;
        IdList stops;
    };

    using StopMap = QMap<int, StopData>;
    using LineMap = QMap<int, LineData>;

    using ScheduleVector = QVector<Schedule>;


private:

    StopMap mStops;
    LineMap mLines;
    ScheduleVector mSchedules;
    QGeoCoordinate mStart;
    QGeoCoordinate mEnd;

    float mWalkingDist;

};

#endif
