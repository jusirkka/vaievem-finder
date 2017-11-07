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

    using IdVector = QVector<qint64>;
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
    Q_INVOKABLE IdVector stops(const QString& line) const;
    Q_INVOKABLE QColor color(const QString& line) const;
    Q_INVOKABLE QString name(qint64 id) const;
    Q_INVOKABLE QGeoCoordinate location(qint64 id) const;

    ~Timetable();

private:

    class Schedule {
    public:
        qint64 line; // NameMap id
        qint64 origin; // NameMap id
        qint64 destination; // NameMap id
        qint64 leaving; // msecs since epoch
        qint64 arriving; // msecs since epoch
    };

    using NameMap = QMap<qint64, QString>;
    using IdHash = QHash<QString, qint64>;
    using ColorMap = QMap<qint64, QColor>;
    using IdVectorMap = QMap<qint64, IdVector>;
    using IdVectorMapIterator = QMapIterator<qint64, IdVector>;
    using CoordMap = QMap<qint64, QGeoCoordinate>;
    using ScheduleVector = QVector<Schedule>;

private:

    NameMap mLineNames;
    IdHash mLineIds;
    ColorMap mLineColors;
    IdVectorMap mStops;
    NameMap mStopNames;
    CoordMap mStopLocations;
    ScheduleVector mSchedules;

};

#endif
