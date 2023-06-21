#ifndef MYDB_H
#define MYDB_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QDebug>
#include <QSqlError>


class MyDB
{
public:
    //constructor
    MyDB();

    //methods
    static MyDB *instance;
    void init();
    QSqlDatabase db;
    static MyDB* getInstance();
    static void ResetInstance();
    QSqlDatabase getDBInstance();

    //destructor
    ~MyDB();
};

#endif // MYDB_H
