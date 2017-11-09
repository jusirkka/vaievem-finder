#include <QDir>
#include <QStandardPaths>
#include <QSqlRecord>
#include <QVariant>
#include <QSqlError>
#include <QDebug>

#include "database.h"

Database::Database() {

    m_DB = QSqlDatabase::addDatabase("QSQLITE");
    QStringList locs = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    for (QString loc: locs) {
        qDebug() << loc;
        QString timetable = QString("%1/timetable.db").arg(loc);
        if (QFileInfo(timetable).isReadable()) {
            m_DB.setDatabaseName(timetable);
            m_DB.open();
            break;
        }
    }

    if (!m_DB.isOpen()) {
        qFatal("Cannot find readable timetable.db in standard app data locations");
    }

    m_DB.close();
}

Database* Database::instance() {
    static Database* db = new Database();
    return db;
}

QSqlQuery& Database::exec(const QString& sql) {
    if (!m_DB.isOpen()) m_DB.open();
    m_Query = QSqlQuery(m_DB);
    // qDebug() << "exec" << sql;
    m_Query.exec(sql);
    if (m_Query.lastError().isValid()) qWarning() << m_Query.lastError();
    return m_Query;
}


QSqlQuery& Database::prepare(const QString& sql) {
    if (!m_DB.isOpen()) m_DB.open();
    m_Query = QSqlQuery(m_DB);
    // qDebug() << "prepare" << sql;
    m_Query.prepare(sql);
    if (m_Query.lastError().isValid()) qWarning() << m_Query.lastError();
    return m_Query;
}

static void exec_and_trace(QSqlQuery& r) {
    r.exec();
    if (r.lastError().isValid()) qWarning() << r.lastError();
}


Database::RowVector Database::Query(const QString& sql, const VariableVector& vars) {
    QSqlQuery r;
    if (vars.isEmpty()) {
        r = instance()->exec(sql);
    } else {
        r = instance()->prepare(sql);
        for (int i = 0; i < vars.size(); ++i) {
            r.bindValue(i, vars[i]);
        }
        exec_and_trace(r);
    }
    RowVector s;
    while (r.next()) {
        Row row(r.record().count());
        for (int i = 0; i < r.record().count(); ++i) {
            // qDebug() << r.value(i);
            row[i] = r.value(i);
        }
        s << row;
    }
    return s;
}
