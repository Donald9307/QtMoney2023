#include "mainwindow.h"
#include <QApplication>
#include <QtSql/QSqlQuery>
#include <QTabWidget>
#include <QPluginLoader>
#include <QVariant>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.loadBankData();
    w.loadAccountData();
    w.loadCCAccountData();
    w.show();


    return a.exec();
}

