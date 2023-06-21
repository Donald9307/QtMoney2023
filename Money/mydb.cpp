#include "mydb.h"

MyDB* MyDB::instance = nullptr;

MyDB::MyDB()
{
    init();
}

void MyDB::init()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "mydb.sqlite3");
    db.setDatabaseName("/home/donald/Qt6ci/build-Money-Desktop-Debug/mydb.sqlite3");

    if(QFile::exists("/home/donald/Qt6ci/build-Money-Desktop-Debug/mydb.sqlite3"))
            qDebug() << "DB file exists";
        else
           qDebug() << "DB file doesn't exist";

        if (!db.open())
            qDebug() << db.lastError().text();
        else
            qDebug() << "Database loaded successfully!";

}

MyDB *MyDB::getInstance()
{
    qDebug() << "in MyDB::getInstance()";

    if(instance == nullptr)
        instance = new MyDB();

    return instance;

}

QSqlDatabase MyDB::getDBInstance()
{
    return db;
}

void MyDB::ResetInstance()
   {
      delete instance;
      instance = nullptr;
   }

MyDB::~MyDB()
{
   db.close();
}
