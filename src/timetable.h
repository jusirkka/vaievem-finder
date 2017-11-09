#ifndef TIMETABLES_H
#define TIMETABLES_H

#include <QAbstractListModel>
#include <QtPositioning/QGeoCoordinate>
#include <QColor>


class Timetable: public QAbstractListModel
{
    Q_OBJECT

public:

    enum TimetableRoles {
        LineNameRole = Qt::UserRole + 1,
        LineColorRole,
        OriginRole,
        DestinationRole,
        LeavingRole,
        ArrivingRole
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
    Q_INVOKABLE QStringList lines() const;
    Q_INVOKABLE QList<int> stops(const QString& line) const;
    Q_INVOKABLE QColor color(const QString& line) const;
    Q_INVOKABLE QString name(int id) const;
    Q_INVOKABLE QGeoCoordinate location(int id) const;

    ~Timetable();

private:

    class Schedule {
    public:
        int line; // NameMap id
        int origin; // NameMap id
        int destination; // NameMap id
        qint64 leaving; // msecs since epoch
        qint64 arriving; // msecs since epoch
    };

    using NameMap = QMap<int, QString>;
    using IdHash = QHash<QString, int>;
    using ColorMap = QMap<int, QColor>;
    using IdListMap = QMap<int, IdList>;
    using IdListMapIterator = QMapIterator<int, IdList>;
    using CoordMap = QMap<int, QGeoCoordinate>;
    using ScheduleVector = QVector<Schedule>;

private:

    NameMap mLineNames;
    IdHash mLineIds;
    ColorMap mLineColors;
    IdListMap mStops;
    NameMap mStopNames;
    CoordMap mStopLocations;
    ScheduleVector mSchedules;

};

#endif
