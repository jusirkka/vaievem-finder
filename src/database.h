#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>

using db_int_t = qint64; // allow -1 to represent error

class Database {

public:

    using VariableVector = QVector<QVariant>;

    using Row = QVector<QVariant>;
    using RowVector = QVector<Row>;

    static RowVector Query(const QString& sql, const VariableVector& vars = VariableVector());

private:

    static Database* instance();
    Database();
    Database(const Database&);
    Database& operator=(const Database&);

    QSqlQuery& exec(const QString& query);
    QSqlQuery& prepare(const QString& query);

private:

    QSqlDatabase m_DB;
    QSqlQuery m_Query;

};

#endif // DATABASE_H
