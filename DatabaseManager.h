#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class DatabaseManager {
public:
    DatabaseManager();
    void createTables();
    void insertTestData();

private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
