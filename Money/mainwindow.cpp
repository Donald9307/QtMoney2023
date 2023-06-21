#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlResult>
#include <QAbstractItemModel>
#include <QSqlTableModel>
#include <QDebug>
#include <QtSql>
#include <QMessageBox>
#include <QComboBox>
#include <QString>
#include <QStringList>
#include <QTableView>
#include <algorithm>
#include <QtGui>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadBankData() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
        QString querySelectBankData = "SELECT bkRef, name FROM banks ORDER BY bkRef ASC;";

        QSqlQuery query(querySelectBankData);
        while(query.next()){
            QString bankEntry = query.value(0).toString() + " " + query.value(1).toString();
            ui->comboBoxBkRef->addItem(bankEntry);
            ui->comboBoxBkRef_2->addItem(bankEntry);
            ui->comboBoxBkRef_3->addItem(bankEntry);
            ui->comboBoxBkRef_5->addItem(bankEntry);
        }
    }
    db.close();
}

void MainWindow::loadAccountData() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
        QString querySelectAccountData = "SELECT acctRef, name, bkRef FROM accounts ORDER BY bkRef ASC;";

        QSqlQuery query(querySelectAccountData);
        while(query.next()){
            QString accountEntry = query.value(0).toString() + " " + query.value(1).toString();
            ui->comboBoxAcctRef_2->addItem(accountEntry);
            ui->comboBoxExpAcct->addItem(accountEntry);
        }
    }
    db.close();
}

void MainWindow::loadCCAccountData() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
        QString querySelectCCAccountData = "SELECT ccRef, desc FROM cc_account ORDER BY ccRef ASC;";

        QSqlQuery query(querySelectCCAccountData);
        while(query.next()){
            QString ccAccountEntry = query.value(0).toString() + " " + query.value(1).toString();
            ui->comboBoxCCAcct->addItem(ccAccountEntry);
            ui->comboBoxCCAcct_2->addItem(ccAccountEntry);
        }
    }
    db.close();
}

void MainWindow::on_pBExit_clicked()
{
    exit(0);
}

void MainWindow::on_pBNBankAsset_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_pBNExpAcct_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void MainWindow::on_pBNCCardAcct_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void MainWindow::on_pBNExpTrans_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void MainWindow::on_pBPCAcct_clicked()
{
    ui->tabWidget->setCurrentIndex(10);
}

void MainWindow::on_pBNCreditTrans_clicked()
{
    ui->tabWidget->setCurrentIndex(9);
}

void MainWindow::on_pBLAssets_clicked()
{
    ui->tabWidget->setCurrentIndex(7);
}

void MainWindow::on_pBLCCAccts_clicked()
{
    ui->tabWidget->setCurrentIndex(8);
}

void MainWindow::on_pushButtonSaveBanks_clicked()
{
    int bkRef;
    QString name;
    QString created;
    QString balance;
    QString active;
    QMessageBox msgBox;
    QMessageBox msgBox2;

    msgBox.setText("Bank Data Added!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox2.setText("Transaction Data Added!");
    msgBox2.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
        QString queryTableCreateString = "CREATE IF NOT EXISTS TABLE banks(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                         "bkRef INTEGER, name VARCHAR(30), created VARCHAR(8), balance VARCHAR(15), active VARCHAR(5)";
        QString queryInsertDataString = "INSERT INTO banks(bkRef, name, created, balance, active) VALUES (:bkRef, :name, :created, :balance, :active)";
        QString queryInsertDataString2 = "INSERT INTO transactions(bkRef, acctRef, txRef, txDate, desc, memo, amount) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount)";

        QString queryFindMaxbkRef = "SELECT MAX(bkRef) FROM banks;";

        QSqlQuery query;
        query.exec(queryTableCreateString);
        if(query.exec())
            qDebug() << "Table Added";
        else {
            qDebug() << "Table Wasn't Added";
        }

        QSqlQuery query1(queryFindMaxbkRef);
        while (query1.next()){
        QString maxBkRef = query1.value(0).toString();
        int maxBkRefInt = maxBkRef.toInt();
        int nextBkRefInt = maxBkRefInt+1;
        QString nextBkRef = QString::number(nextBkRefInt);

        ui->lineEditBkRef->setText(nextBkRef);
        ui->lineEditBkRef->displayText();
        }

        QSqlQuery query2(queryInsertDataString);
        query2.prepare(queryInsertDataString);
        query2.bindValue(":bkRef", ui->lineEditBkRef->text().toInt(), QSql::In);
        query2.bindValue(":name", ui->lineEditName->text(), QSql::In);
        query2.bindValue(":created", ui->lineEditCreated->text(), QSql::In);
        query2.bindValue(":balance", ui->lineEditBalance->text(), QSql::In);
        query2.bindValue(":active", "true", QSql::In);
        if(query2.exec()) {
            int ret = msgBox.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qDebug() << "Error: Data not added!";
        }

        QSqlQuery query3(queryInsertDataString2);
        query3.prepare(queryInsertDataString2);
        query3.bindValue(":bkRef", ui->lineEditBkRef->text().toInt(), QSql::In);
        query3.bindValue(":acctRef", 0, QSql::In);
        query3.bindValue(":txRef", 0, QSql::In);
        query3.bindValue(":txDate", ui->lineEditCreated->text(), QSql::In);
        query3.bindValue(":desc", "", QSql::In);
        query3.bindValue(":memo", "", QSql::In);
        query3.bindValue(":amount", ui->lineEditBalance->text(), QSql::In);
        if(query3.exec()) {
            int ret = msgBox2.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qDebug() << "Error: Data not added!";
        }
        ui->comboBoxBkRef_5->clear();
        ui->comboBoxBkRef_2->clear();
        ui->comboBoxAcctRef_2->clear();
        ui->comboBoxBkRef->clear();
        ui->comboBoxCCAcct->clear();
        ui->comboBoxExpAcct->clear();
        ui->comboBoxBkRef_3->clear();
        ui->comboBoxCCAcct_2->clear();
        loadBankData();
    }
    db.close();
}

void MainWindow::on_pushButtonSaveAccts_clicked()
{
    int bkRef;
    int acctRef;
    QString name;
    QString budget;
    QMessageBox msgBox;
    QMessageBox msgBox2;

    msgBox.setText("Expense Account Data Added!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox2.setText("Transaction Data Added!");
    msgBox2.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
        QString queryTableCreateString = "CREATE IF NOT EXISTS TABLE accounts (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                         "bkRef INTEGER, acctRef INTEGER, name VARCHAR(30), budget VARCHAR(15)";
        QString queryInsertDataString = "INSERT INTO accounts (bkRef, acctRef, name, budget) VALUES (:bkRef, :acctRef, :name, :budget)";
        QString queryInsertDataString2 = "INSERT INTO transactions(bkRef, acctRef, txRef, txDate, desc, memo, amount) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount)";

        QString queryFindMaxAcctRef = "SELECT MAX(acctRef) FROM accounts;";
        QString queryFindMaxCCRef = "SELECT MAX(CCRef) FROM cc_account;";

        QSqlQuery query;

        query.exec(queryTableCreateString);
        if(query.exec())
            qDebug() << "Table Added";
        else {
            qDebug() << "Table Wasn't Added";
        }

        int nextARef = 0;
        int nextCRef = 0;
        QString nextAcctRefStr = "";
        QString nextCCRefStr = "";

        QSqlQuery query2(queryFindMaxAcctRef);
        while (query2.next()){
        QString maxAcctRef = query2.value(0).toString();
        int maxAcctRefInt = maxAcctRef.toInt();
        int nextAcctRefInt = maxAcctRefInt+1;
        QString nextAcctRef = QString::number(nextAcctRefInt);
        nextARef = maxAcctRefInt;
        nextAcctRefStr = nextAcctRef;
        }

        QSqlQuery query4(queryFindMaxCCRef);
        while (query4.next()){
        QString maxCCRef = query4.value(0).toString();
        int maxCCRefInt = maxCCRef.toInt();
        int nextCCRefInt = maxCCRefInt+1;
        QString nextCCRef = QString::number(nextCCRefInt);
        nextCCRefStr = nextCCRef;
        nextCRef = maxCCRefInt;
        }

        if(nextARef > nextCRef) {
            ui->lineEditAcctRef->setText(nextAcctRefStr);
        }
        else {
            ui->lineEditAcctRef->setText(nextCCRefStr);
        }

        QSqlQuery query3;

        QString currentBank = ui->comboBoxBkRef_5->currentText();
        QString currentBankSliced = currentBank.sliced(0,2);
        int currentBankSlicedInt = currentBankSliced.toInt();

        query3.prepare(queryInsertDataString);
        query3.bindValue(":bkRef", currentBankSlicedInt, QSql::In);
        query3.bindValue(":acctRef", ui->lineEditAcctRef->text().toInt(), QSql::In);
        query3.bindValue(":name", ui->lineEditName_2->text(), QSql::In);
        query3.bindValue(":budget", "", QSql::In);

        if(query3.exec()) {
            int ret = msgBox.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qDebug() << "Error: Data not added!";
        }

        QSqlQuery query5(queryInsertDataString2);

        query5.prepare(queryInsertDataString2);
        query5.bindValue(":bkRef", currentBankSlicedInt, QSql::In);
        query5.bindValue(":acctRef", ui->lineEditAcctRef->text().toInt(), QSql::In);
        query5.bindValue(":txRef", 0, QSql::In);
        query5.bindValue(":txDate","", QSql::In);
        query5.bindValue(":desc", "", QSql::In);
        query5.bindValue(":memo", "", QSql::In);
        query5.bindValue(":amount", "0", QSql::In);
        if(query5.exec()) {
            int ret = msgBox2.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qDebug() << "Error: Data not added!";
        }
    }
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef_5->clear();
    ui->comboBoxBkRef_2->clear();
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef->clear();
    ui->comboBoxCCAcct->clear();
    ui->comboBoxExpAcct->clear();
    ui->comboBoxBkRef_3->clear();
    ui->comboBoxCCAcct_2->clear();
    loadAccountData();
    loadBankData();
    loadCCAccountData();
    db.close();
}

void MainWindow::on_pushButtonSaveCCAcct_clicked()
{
    int ccRef;
    QMessageBox msgBox;
    QMessageBox msgBox2;

    msgBox.setText("Credit Card Data Added!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox2.setText("Transaction Data Added!");
    msgBox2.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
        QString queryTableCreateString = "CREATE IF NOT EXISTS TABLE cc_account (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                         "ccRef INTEGER";
        QString queryFindMaxAcctRef = "SELECT MAX(acctRef) FROM accounts;";
        QString queryFindMaxCCRef = "SELECT MAX(ccRef) FROM cc_account;";
        QString queryInsertDataString = "INSERT INTO cc_account (ccRef, desc) VALUES (:ccRef, :desc);";
        QString queryInsertDataString2 = "INSERT INTO transactions(bkRef, acctRef, txRef, txDate, desc, memo, amount) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount)";


        int nextARef = 0;
        int nextCRef = 0;
        QString nextAcctRefStr = "";
        QString nextCCRefStr = "";

        QSqlQuery query;

        query.exec(queryTableCreateString);
        if(query.exec())
            qDebug() << "Table Added";
        else {
            qDebug() << "Table Wasn't Added";
        }

        QSqlQuery query2(queryFindMaxCCRef);

        while (query2.next()){
        int maxCCRef = query2.value(0).toInt();
        int nextCCRefInt = maxCCRef +1;
        nextCCRefStr = QString::number(nextCCRefInt);
        nextCRef = nextCCRefInt;
        }

        QSqlQuery query5(queryFindMaxAcctRef);
        while (query5.next()){
        int maxAcctRef = query5.value(0).toInt();
        int nextAcctRefInt = maxAcctRef+1;
        nextAcctRefStr = QString::number(nextAcctRefInt);
        nextARef = nextAcctRefInt;
        }

        if(nextARef > nextCRef) {
            ui->lineEditCreditAcct->setText(nextAcctRefStr);
        }
        else {
            ui->lineEditCreditAcct->setText(nextCCRefStr);
        }

        QSqlQuery query3(queryInsertDataString);
        query3.prepare(queryInsertDataString);
        query3.bindValue(":ccRef", ui->lineEditCreditAcct->text().toInt(), QSql::In);
        query3.bindValue(":desc", ui->lineEditNewCCAcct->text(), QSql::In);

        if(query3.exec()) {
            int ret = msgBox.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qDebug() << "Error: Data not added!";
        }       

        QSqlQuery query4(queryInsertDataString2);

        query4.prepare(queryInsertDataString2);
        query4.bindValue(":bkRef", ui->lineEditBkRef->text().toInt(), QSql::In);
        query4.bindValue(":acctRef", ui->lineEditCreditAcct->text().toInt(), QSql::In);
        query4.bindValue(":txRef", 0, QSql::In);
        query4.bindValue(":txDate", ui->lineEditCreated->text(), QSql::In);
        query4.bindValue(":desc", "", QSql::In);
        query4.bindValue(":memo", "", QSql::In);
        query4.bindValue(":amount", "0", QSql::In);
        if(query4.exec()) {
            int ret = msgBox2.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qDebug() << "Error: Data not added!";
        }
    }
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef_5->clear();
    ui->comboBoxBkRef_2->clear();
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef->clear();
    ui->comboBoxCCAcct->clear();
    ui->comboBoxExpAcct->clear();
    ui->comboBoxBkRef_3->clear();
    ui->comboBoxCCAcct_2->clear();
    loadAccountData();
    loadBankData();
    loadCCAccountData();
    db.close();
}

void MainWindow::on_pushButtonSaveExpTx_clicked()
{
    int bkRef;
    int acctRef;
    int txRef;
    QString txDate;
    QString desc;
    QString memo;
    QString amount;
    QMessageBox msgBox;

    msgBox.setText("Data Added!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        qDebug() << db.lastError();
    } else {
        qDebug() << "Database: connection ok";
        QString queryTableCreateString = "CREATE IF NOT EXISTS TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                         "bkRef INTEGER, acctRef INTEGER, txRef INTEGER, txDate VARCHAR(8), desc VARCHAR(30), memo VARCHAR(30), amount VARCHAR(15)";
        QString queryFindMaxTxRef = "SELECT MAX(txRef) FROM transactions;";
        QString queryInsertDataString = "INSERT INTO transactions (bkRef, acctRef, txRef, txDate, desc, memo, amount) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount)";

        QSqlQuery query2(queryFindMaxTxRef);
        while (query2.next()){
        QString maxTxRef = query2.value(0).toString();
        int maxTxRefInt = maxTxRef.toInt();
        int nextTxRefInt = maxTxRefInt +1;
        QString nextTxRef = QString::number(nextTxRefInt);

        ui->lineEditTxRef->setText(nextTxRef);
        ui->lineEditTxRef->displayText();

        ui->lineEditExpAcct->setText(nextTxRef);
        ui->lineEditExpAcct->displayText();
        }

        QSqlQuery query3(queryTableCreateString);
        if(query3.exec())
            qDebug() << "Table Added";
        else {
            qDebug() << "Table Wasn't Added";
        }

        QSqlQuery query4;

        QString currentBank = ui->comboBoxBkRef_2->currentText();
        QString currentBankSliced = currentBank.sliced(0,2);

        QString currentAccount = ui->comboBoxAcctRef_2->currentText();
        QString currentAccountSliced = currentAccount.sliced(0,2);

        query4.prepare(queryInsertDataString);
        query4.bindValue(":bkRef", currentBankSliced.toInt(), QSql::In);
        query4.bindValue(":acctRef", currentAccountSliced.toInt(), QSql::In);
        query4.bindValue(":txRef", ui->lineEditTxRef->text().toInt(), QSql::In);
        query4.bindValue(":txDate", ui->lineEditDate->text(), QSql::In);
        query4.bindValue(":desc", ui->lineEditDesc->text(), QSql::In);
        query4.bindValue(":memo", ui->lineEditMemo->text(), QSql::In);
        query4.bindValue(":amount", ui->lineEditAmount->text(), QSql::In);

        if(query4.exec()) {
            int ret = msgBox.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qDebug() << "Error: Data not added!";
        }
    }
    ui->comboBoxBkRef_5->clear();
    ui->comboBoxBkRef_2->clear();
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef->clear();
    ui->comboBoxCCAcct->clear();
    ui->comboBoxExpAcct->clear();
    ui->comboBoxBkRef_3->clear();
    ui->comboBoxCCAcct_2->clear();
    loadAccountData();
    loadBankData();
    loadCCAccountData();
    db.close();
}

void MainWindow::on_pushButtonSaveCCTx_clicked()
{
    int id;
    int bkRef;
    int acctRef;
    int txRef;
    QString txDate;
    QString desc;
    QString memo;
    QString amount;
    QMessageBox msgBox1;
    QMessageBox msgBox2;
    QMessageBox msgBox3;

    msgBox1.setText("Bank Data Added!");
    msgBox1.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox2.setText("Expense Account Data Added!");
    msgBox2.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox3.setText("Offset Data Added!");
    msgBox3.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
        QString queryTableCreateString = "CREATE IF NOT EXISTS TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                         "bkRef INTEGER, acctRef INTEGER, txRef INTEGER, txDate VARCHAR(8), desc VARCHAR(30), memo VARCHAR(30), amount VARCHAR(15)";
        QString queryFindMaxTxRef = "SELECT MAX(txRef) FROM transactions;";
        QString queryInsertDataString1 = "INSERT INTO transactions (bkRef, acctRef, txRef, txDate, desc, memo, amount) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount)";
        QString queryInsertDataString2 = "INSERT INTO transactions (bkRef, acctRef, txRef, txDate, desc, memo, amount) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount)";
        QString queryInsertDataString3 = "INSERT INTO transactions (bkRef, acctRef, txRef, txDate, desc, memo, amount) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount)";
        QString queryFindMaxID = "SELECT MAX(id) FROM transactions;";

        QSqlQuery query2(queryFindMaxTxRef);
        while (query2.next()){
        QString maxTxRef = query2.value(0).toString();
        int maxTxRefInt = maxTxRef.toInt();
        int nextTxRefInt = maxTxRefInt +1;
        QString nextTxRef = QString::number(nextTxRefInt);

        ui->lineEditTxRef_2->setText(nextTxRef);
        ui->lineEditTxRef_2->displayText();
        }

        QSqlQuery query3(queryTableCreateString);
        if(query3.exec())
            qDebug() << "Table Added";
        else {
            qDebug() << "Table Wasn't Added";
        }

        QString currentBank = ui->comboBoxBkRef->currentText();
        QString currentBankSliced = currentBank.sliced(0,2);

        QString creditCardAccount = ui->comboBoxCCAcct->currentText();
        QString creditCardAccountSliced = creditCardAccount.sliced(0,2);

        QSqlQuery query4(queryInsertDataString1);
        query4.prepare(queryInsertDataString1);
        query4.bindValue(":bkRef", currentBankSliced.toInt(), QSql::In);
        query4.bindValue(":acctRef", creditCardAccountSliced.toInt(), QSql::In);
        query4.bindValue(":txRef", ui->lineEditTxRef->text().toInt(), QSql::In);
        query4.bindValue(":txDate", ui->lineEditDate_2->text(), QSql::In);
        query4.bindValue(":desc", ui->lineEditDesc_2->text(), QSql::In);
        query4.bindValue(":memo", ui->lineEditMemo_2->text(), QSql::In);
        query4.bindValue(":amount", ui->lineEditAmount_2->text(), QSql::In);
        if(query4.exec()){
            int ret = msgBox1.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        qInfo() << "Data Saved to Database";

        QString expenseAccount = ui->comboBoxExpAcct->currentText();
        QString expenseAccountSliced = expenseAccount.sliced(0,2);
        int txRefPlusOne = ui->lineEditTxRef->text().toInt()+1;

        QSqlQuery query5(queryInsertDataString2);
        query5.prepare(queryInsertDataString2);
        query5.bindValue(":bkRef", currentBankSliced.toInt(), QSql::In);
        query5.bindValue(":acctRef", expenseAccountSliced.toInt(), QSql::In);
        query5.bindValue(":txRef", txRefPlusOne, QSql::In);
        query5.bindValue(":txDate", ui->lineEditDate_2->text(), QSql::In);
        query5.bindValue(":desc", ui->lineEditDesc_2->text(), QSql::In);
        query5.bindValue(":memo", ui->lineEditMemo_2->text(), QSql::In);
        query5.bindValue(":amount", ui->lineEditAmount_2->text(), QSql::In);

        if(query5.exec()){
            int ret = msgBox2.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        qInfo() << "Data Saved to Database";

        QString offSetText = ui->lineEditAmount_2->text();
        int offSet = -2 * offSetText.toInt();
        QString offSetString = QString::number(offSet);

        QSqlQuery query6(queryInsertDataString3);
        query6.prepare(queryInsertDataString3);
        query6.bindValue(":bkRef", currentBankSliced, QSql::In);
        query6.bindValue(":acctRef", 0, QSql::In);
        query6.bindValue(":txRef", txRefPlusOne, QSql::In);
        query6.bindValue(":txDate", ui->lineEditDate->text(), QSql::In);
        query6.bindValue(":desc", "0", QSql::In);
        query6.bindValue(":memo", "0", QSql::In);
        query6.bindValue(":amount", offSetString, QSql::In);

        if(query6.exec()){
            int ret = msgBox3.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        qInfo() << "Data Saved to Database";

        QSqlQuery query7(queryFindMaxID);
        while (query7.next()){
        int maxID = query7.value(0).toInt();
        QString maxIDStr = QString::number(maxID);
        ui->lineEditMaxID->setText(maxIDStr);
        ui->lineEditMaxID->displayText();
        }
    }
    ui->comboBoxBkRef_5->clear();
    ui->comboBoxBkRef_2->clear();
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef->clear();
    ui->comboBoxCCAcct->clear();
    ui->comboBoxExpAcct->clear();
    ui->comboBoxBkRef_3->clear();
    ui->comboBoxCCAcct_2->clear();
    loadAccountData();
    loadBankData();
    loadCCAccountData();
    db.close();
}

void MainWindow::on_pushButtonDispAssets_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QString queryBanks = "Select bkRef FROM banks WHERE active == 'true';";
        QString queryBankBalances = "SELECT bkRef, SUM(amount) FROM transactions \
                WHERE bkRef LIKE :bkRef \
                UNION \
                SELECT bkRef, name FROM banks \
                WHERE bkRef LIKE :bkRef;";

        QSqlQuery query1(queryBanks);
        QSqlQuery query(queryBankBalances);

        int sum = 0;
        int bankBalanceInt;

        while(query1.next()){
            query.prepare(queryBankBalances);
            query.bindValue(":bkRef", query1.value(0), QSql::InOut);
            if(!query.exec()){
                qInfo() << "Query didn't execute!";
                qInfo() << query.lastError();
            }
            else {
                query.next();
                QString bankAccount = query.value(0).toString();
                QString bankBalance = query.value(1).toString();

                bankBalanceInt = bankBalance.toInt();
                sum += bankBalanceInt;
                query.next();

                QString name = query.value(1).toString();
                QString result = bankAccount + " " + name;
                ui->textEditBankList->append(result);

                if(bankBalanceInt !=0) {
                    double bankBalanceDouble = bankBalanceInt/100;
                    QString bankBalanceDoubleString = QString::number(bankBalanceDouble);

                    QString bankBalanceDecimalString = bankBalance.sliced((bankBalance.length()-2), 2);
                    QString balance = '$' + bankBalanceDoubleString + '.' + bankBalanceDecimalString;
                    ui->textEditBankBalances->append(balance);
                }
                else {
                    ui->textEditBankBalances->append('$' + bankBalance + '.' + "00");
                }

                QString sumString = QString::number(sum);
                if(sum !=0) {
                    double sumDouble = sum/100;
                    QString sumDoubleString = QString::number(sumDouble);
                    QString sumDecimalString = sumString.sliced((sumString.length()-2), 2);
                    QString sumOfAccounts = '$' + sumDoubleString + '.' + sumDecimalString;
                    ui->lineEditSumOfAssets->setText(sumOfAccounts);
                }
                else {
                    ui->lineEditSumOfAssets->setText('$' + sumString + '.' + "00");
                }
            }
        }
    }
    db.close();
}

void MainWindow::on_pushButtonSavePayCCTx_2_clicked()
{
    int bkRef;
    int acctRef;
    int txRef;
    QString txDate;
    QString desc;
    QString memo;
    QString amount;
    QMessageBox msgBox1;
    QMessageBox msgBox2;

    msgBox1.setText("Bank Data Added!");
    msgBox1.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox2.setText("Credit Card Data Added!");
    msgBox2.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
        QString queryTableCreateString = "CREATE IF NOT EXISTS TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                         "bkRef INTEGER, acctRef INTEGER, txRef INTEGER, txDate VARCHAR(8), desc VARCHAR(30), memo VARCHAR(30), amount VARCHAR(15)";
        QString queryFindMaxTxRef = "SELECT MAX(txRef) FROM transactions;";
        QString queryInsertDataString = "INSERT INTO transactions (bkRef, acctRef, txRef, txDate, desc, memo, amount) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount)";
        QString queryInsertDataString1 = "INSERT INTO transactions (bkRef, acctRef, txRef, txDate, desc, memo, amount) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount)";


        QSqlQuery query2(queryFindMaxTxRef);
        while (query2.next()){
        QString maxTxRef = query2.value(0).toString();
        int maxTxRefInt = maxTxRef.toInt();
        int nextTxRefInt = maxTxRefInt +1;
        QString nextTxRef = QString::number(nextTxRefInt);

        ui->lineEditTxRef_3->setText(nextTxRef);
        ui->lineEditTxRef_3->displayText();
        }

        QSqlQuery query3(queryTableCreateString);
        if(query3.exec())
            qDebug() << "Table Added";
        else {
            qDebug() << "Table Wasn't Added";
        }

        QSqlQuery query4(queryInsertDataString);

        QString currentBank = ui->comboBoxBkRef_3->currentText();
        QString currentBankSliced = currentBank.sliced(0,2);

        QString currentCCAccount = ui->comboBoxCCAcct_2->currentText();
        QString currentCCAccountSliced = currentCCAccount.sliced(0,2);

        QString amount = ui->lineEditAmount_3->text();
        int CCOffsetAmount = -1 * amount.toInt();
        QString CCOffsetAmountString = QString::number(CCOffsetAmount);

        query4.prepare(queryInsertDataString);
        query4.bindValue(":bkRef", currentBankSliced.toInt(), QSql::In);
        query4.bindValue(":acctRef", currentCCAccountSliced.toInt(), QSql::In);
        query4.bindValue(":txRef", ui->lineEditTxRef_3->text().toInt(), QSql::In);
        query4.bindValue(":txDate", ui->lineEditDate_3->text(), QSql::In);
        query4.bindValue(":desc", ui->lineEditDesc_3->text(), QSql::In);
        query4.bindValue(":memo", ui->lineEditMemo_3->text(), QSql::In);
        query4.bindValue(":amount", CCOffsetAmountString, QSql::In);

        if(query4.exec()) {
            int ret = msgBox1.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qDebug() << "Error: Data not added!";
        }

        QSqlQuery query5(queryInsertDataString1);

        int BankOffsetAmount = 2 * amount.toInt();
        QString BankOffsetAmountString = QString::number(BankOffsetAmount);

        query5.prepare(queryInsertDataString1);
        query5.bindValue(":bkRef", currentBankSliced.toInt(), QSql::In);
        query5.bindValue(":acctRef", 0, QSql::In);
        query5.bindValue(":txRef", ui->lineEditTxRef_3->text().toInt(), QSql::In);
        query5.bindValue(":txDate", "0", QSql::In);
        query5.bindValue(":desc", "0", QSql::In);
        query5.bindValue(":memo", "0", QSql::In);
        query5.bindValue(":amount", BankOffsetAmountString, QSql::In);

        if(query5.exec()) {
            int ret = msgBox2.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qDebug() << "Error: Data not added!";
        }
    }
    ui->comboBoxBkRef_5->clear();
    ui->comboBoxBkRef_2->clear();
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef->clear();
    ui->comboBoxCCAcct->clear();
    ui->comboBoxExpAcct->clear();
    ui->comboBoxBkRef_3->clear();
    ui->comboBoxCCAcct_2->clear();
    loadAccountData();
    loadBankData();
    loadCCAccountData();
    db.close();
}

void MainWindow::on_pushButtonCCBalances_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QString queryAccountItems = "Select ccRef FROM cc_account;";
        QString queryAccountBalances = "SELECT acctRef, SUM(amount) FROM transactions \
                WHERE acctRef LIKE :acctRef \
                UNION \
                SELECT ccRef, desc FROM cc_account \
                WHERE ccRef LIKE :ccRef;";

        QSqlQuery query1(queryAccountItems);
        QSqlQuery query(queryAccountBalances);

        int ccBalanceInt = 0;
        int sum = 0;

        query1.exec();
        while(query1.next()){
            query.prepare(queryAccountBalances);
            query.bindValue(":acctRef", query1.value(0), QSql::InOut);
            query.bindValue(":ccRef", query1.value(0), QSql::InOut);
            if(!query.exec()){
                qInfo() << "Query didn't execute!";
                qInfo() << query.lastError();
            }
            else {
                query.next();
                QString ccAccount = query.value(0).toString();
                QString ccBalance = query.value(1).toString();
                ccBalanceInt = ccBalance.toInt();
                sum += ccBalanceInt;
                query.next();

                QString name = query.value(1).toString();
                QString result = ccAccount + " " + name;
                ui->textEditCCAcct->append(result);

                if(ccBalanceInt !=0) {
                    double ccBalanceDouble = ccBalanceInt/100;
                    QString ccBalanceDoubleString = QString::number((ccBalanceDouble));
                    QString ccBalanceDecimalString = ccBalance.sliced((ccBalance.length()-2), 2);
                    QString accountBalance = '$' + ccBalanceDoubleString + '.' + ccBalanceDecimalString;
                    ui->textEditBalances->append(accountBalance);
                    }
                    else  {
                        ui->textEditBalances->append('$' + ccBalance + '.' + "00");
                    }

                QString sumString = QString::number(sum);
                if(sum !=0) {
                    double sumDouble = sum/100;
                    QString sumDoubleString = QString::number(sumDouble);
                    QString sumDecimalString = sumString.sliced((sumString.length()-2), 2);
                    QString sumOfAccounts = '$' + sumDoubleString + '.' + sumDecimalString;
                    ui->lineEditTotalCCBal->setText(sumOfAccounts);
                }
                else {
                    ui->lineEditTotalCCBal->setText('$' + sumString + '.' + "00");
                }
            }
        }
    }
    db.close();
}

void MainWindow::on_pushButtonTxByDate_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QString month = ui->comboBoxM->currentText();
        QString day = ui->comboBoxD->currentText();
        QString year = ui->comboBoxY->currentText();

        QSqlQuery query;

        query.prepare("SELECT * FROM transactions WHERE txDate LIKE :txDate;");
        QVariant dateFormatted = (month + day + year);
        qInfo() << "Database connection successful";

        query.bindValue(":txDate", dateFormatted.toString(), QSql::In);
        query.exec();
        query.next();
        if(!query.exec()){
            qInfo() << "Query didn't execute";
        }
        else {
        QSqlQueryModel* model = new QSqlQueryModel();
        model->setQuery(std::move(query));
        ui->tableViewTxByDate->setModel(model);
        }
    }
    db.close();
}

void MainWindow::on_pBVTransByDate_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}

void MainWindow::on_pBPMTransNC_clicked()
{
    ui->tabWidget->setCurrentIndex(11);
}

void MainWindow::on_pushButtonTextSearch_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QSqlQuery query;

        query.prepare("SELECT * FROM transactions WHERE desc LIKE :desc;");
        QVariant TextSearch = ui->lineEditTextSearch->text();

        query.bindValue(":desc", TextSearch, QSql::In);
        query.exec();

        if(!query.exec()){
            qInfo() << "Query didn't execute";
        }
        else {
        QSqlQueryModel* model = new QSqlQueryModel();
        model->setQuery(std::move(query));
        ui->tableViewTxByDate->setModel(model);
        }
    }
    db.close();
}

void MainWindow::on_pushButtonMemExpTx_clicked()
{
    int bkRef;
    int acctRef;
    int txRef;
    QString txDate;
    QString desc;
    QString memo;
    QString amount;
    QMessageBox msgBox;


    msgBox.setText("Data Added!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
        QString queryTableCreateString = "CREATE IF NOT EXISTS TABLE MemTrans (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                         "bkRef INTEGER, acctRef INTEGER, txRef INTEGER, txDate VARCHAR(8), desc VARCHAR(30), memo VARCHAR(30), amount VARCHAR(15)";
        QString queryFindMaxTxRef = "SELECT MAX(txRef) FROM MemTrans;";
        QString queryInsertDataString = "INSERT INTO MemTrans (bkRef, acctRef, txRef, txDate, desc, memo, amount, DOTM, MOTY, MOTY2, MOTY3, MOTY4) \
                                        VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount, :DOTM, :MOTY, :MOTY2, :MOTY3, :MOTY4)";

        QSqlQuery query2(queryFindMaxTxRef);
        while (query2.next()){
        QString maxTxRef = query2.value(0).toString();
        int maxTxRefInt = maxTxRef.toInt();
        int nextTxRefInt = maxTxRefInt +1;
        QString nextTxRef = QString::number(nextTxRefInt);

        ui->lineEditTxRef->setText(nextTxRef);
        ui->lineEditTxRef->displayText();
        }

        QSqlQuery query3(queryTableCreateString);
        if(query3.exec())
            qDebug() << "Table Added";
        else {
            qDebug() << "Table Wasn't Added";
        }

        QSqlQuery query4;

        QString currentBank = ui->comboBoxBkRef_2->currentText();
        QString currentBankSliced = currentBank.sliced(0,2);

        QString currentAccount = ui->comboBoxAcctRef_2->currentText();
        QString currentAccountSliced = currentAccount.sliced(0,2);

        query4.prepare(queryInsertDataString);
        query4.bindValue(":bkRef", currentBankSliced.toInt(), QSql::In);
        query4.bindValue(":acctRef", currentAccountSliced.toInt(), QSql::In);
        query4.bindValue(":txRef", ui->lineEditTxRef->text().toInt(), QSql::In);
        query4.bindValue(":txDate", ui->lineEditDate->text(), QSql::In);
        query4.bindValue(":desc", ui->lineEditDesc->text(), QSql::In);
        query4.bindValue(":memo", ui->lineEditMemo->text(), QSql::In);
        query4.bindValue(":amount", ui->lineEditAmount->text(), QSql::In);
        query4.bindValue(":DOTM", ui->lineEditDOTM->text(), QSql::In);
        query4.bindValue(":MOTY", ui->lineEditMOTY->text(), QSql::In);
        query4.bindValue(":MOTY2", ui->lineEditMOTY2->text(), QSql::In);
        query4.bindValue(":MOTY3", ui->lineEditMOTY3->text(), QSql::In);
        query4.bindValue(":MOTY4", ui->lineEditMOTY4->text(), QSql::In);

        if(query4.exec()) {
            int ret = msgBox.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qDebug() << "Error: Data not added!";
        }
    }
    db.close();
}

void MainWindow::on_pushButtonFindMemExpTx_clicked()
{
    QMessageBox msgBox;

    msgBox.setText("Transaction Not Found!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        qInfo() << "Database connection successful";
        QSqlQuery* query = new QSqlQuery(db);
        QVariant description = ui->lineEditFindMemExpTx->text();
        QVariant expRef, txDate, desc, memo, amount;

        query->prepare("SELECT bkRef, acctRef, expRef, txDate, desc, memo, amount FROM MemTrans WHERE desc == :desc;");
        query->bindValue(":desc", description);

        query->exec();

        if(!query->exec()){
            qInfo() << "Query didn't execute";
        }
        else {
            if(query->next()) {
            ui->lineEditBkRef_3->setText(query->value("bkRef").toString());
            ui->lineEditAcctRef_2->setText(query->value("acctRef").toString());
            ui->lineEditDate->setText(query->value("txDate").toString());
            ui->lineEditDesc->setText(query->value("desc").toString());
            ui->lineEditMemo->setText(query->value("memo").toString());
            ui->lineEditAmount->setText(query->value("amount").toString());
            }
            else {
                msgBox.exec();
            }
        }
        delete query;
    }
    db.close();
}

void MainWindow::on_pushButtonFindMemTx_2_clicked()
{
    QMessageBox msgBox;

    msgBox.setText("Transaction Not Found!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        qInfo() << "Database connection successful";
        QSqlQuery* query = new QSqlQuery(db);
        QVariant description = ui->lineEditFindMemCCTx->text();
        QVariant txDate, desc, memo, amount;

        query->prepare("SELECT bkRef, acctRef, expRef, txDate, desc, memo, amount FROM MemTrans WHERE desc == :desc;");
        query->bindValue(":desc", description);

        query->exec();

        if(!query->exec()){
            qInfo() << "Query didn't execute";
        }
        else {
            if(query->next()) {
            ui->lineEditBkRef_2->setText(query->value("bkRef").toString());
            ui->lineEditCCAcct->setText(query->value("acctRef").toString());
            ui->lineEditExpAcct->setText(query->value("expRef").toString());
            ui->lineEditDate_2->setText(query->value("txDate").toString());
            ui->lineEditDesc_2->setText(query->value("desc").toString());
            ui->lineEditMemo_2->setText(query->value("memo").toString());
            ui->lineEditAmount_2->setText(query->value("amount").toString());
            }
            else {
                msgBox.exec();
            }
        }
        delete query;
    }
    db.close();
}

void MainWindow::on_pushButtonFindMemTx_clicked()
{
    QMessageBox msgBox;

    msgBox.setText("Transaction Not Found!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        qInfo() << "Database connection successful";
        QSqlQuery* query = new QSqlQuery(db);
        QVariant description = ui->lineEditFindMemTx_2->text();
        QVariant txDate, desc, memo, amount;

        query->prepare("SELECT bkRef, acctRef, expRef, txDate, desc, memo, amount FROM MemTrans WHERE desc == :desc;");
        query->bindValue(":desc", description);

        query->exec();

        if(!query->exec()){
            qInfo() << "Query didn't execute";
        }
        else {
            if(query->next()) {
            ui->lineEditbkRef->setText(query->value("bkRef").toString());
            ui->lineEditCCAcct_2->setText(query->value("acctRef").toString());
            ui->lineEditDate_3->setText(query->value("txDate").toString());
            ui->lineEditDesc_3->setText(query->value("desc").toString());
            ui->lineEditMemo_3->setText(query->value("memo").toString());
            ui->lineEditAmount_3->setText(query->value("amount").toString());
            }
            else {
                msgBox.exec();
            }
        }
        delete query;
    }
    db.close();
}

void MainWindow::on_pushButtonMemCCTx_2_clicked()
{
    int bkRef;
    int acctRef;
    int expRef;
    int txRef;
    QString txDate;
    QString desc;
    QString memo;
    QString amount;
    QMessageBox msgBox;


    msgBox.setText("Data Added!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
        QString queryTableCreateString = "CREATE IF NOT EXISTS TABLE MemTrans (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                         "bkRef INTEGER, acctRef INTEGER, txRef INTEGER, txDate VARCHAR(8), desc VARCHAR(30), memo VARCHAR(30), amount VARCHAR(15)";
        QString queryFindMaxTxRef = "SELECT MAX(txRef) FROM MemTrans;";
        QString queryInsertDataString = "INSERT INTO MemTrans (bkRef, acctRef, txRef, txDate, desc, memo, amount, DOTM, MOTY, MOTY2, MOTY3, MOTY4) \
                                        VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount, :DOTM, :MOTY, :MOTY2, :MOTY3, :MOTY4)";

        QSqlQuery query2(queryFindMaxTxRef);
        while (query2.next()){
        QString maxTxRef = query2.value(0).toString();
        int maxTxRefInt = maxTxRef.toInt();
        int nextTxRefInt = maxTxRefInt +1;
        QString nextTxRef = QString::number(nextTxRefInt);

        ui->lineEditTxRef_3->setText(nextTxRef);
        ui->lineEditTxRef_3->displayText();
        }

        QSqlQuery query3(queryTableCreateString);
        if(query3.exec())
            qDebug() << "Table Added";
        else {
            qDebug() << "Table Wasn't Added";
        }

        QSqlQuery query4;

        QString currentBank = ui->comboBoxBkRef->currentText();
        QString currentBankSliced = currentBank.sliced(0,2);

        QString currentCCAccount = ui->comboBoxCCAcct->currentText();
        QString currentCCAccountSliced = currentCCAccount.sliced(0,2);

        QString currentExpAccount = ui->comboBoxExpAcct->currentText();
        QString currentExpAccountSliced = currentExpAccount.sliced(0,2);

        query4.prepare(queryInsertDataString);
        query4.bindValue(":bkRef", currentBankSliced.toInt(), QSql::In);
        query4.bindValue(":acctRef", currentCCAccountSliced.toInt(), QSql::In);
        query4.bindValue(":txRef", ui->lineEditTxRef_2->text().toInt(), QSql::In);
        query4.bindValue(":txDate", ui->lineEditDate_2->text(), QSql::In);
        query4.bindValue(":desc", ui->lineEditDesc_2->text(), QSql::In);
        query4.bindValue(":memo", ui->lineEditMemo_2->text(), QSql::In);
        query4.bindValue(":amount", ui->lineEditAmount_2->text(), QSql::In);
        query4.bindValue(":DOTM", ui->lineEditDOTM_2->text(), QSql::In);
        query4.bindValue(":MOTY", ui->lineEditMOTY_2->text(), QSql::In);
        query4.bindValue(":MOTY2", ui->lineEditMOTY2_2->text(), QSql::In);
        query4.bindValue(":MOTY3", ui->lineEditMOTY3_2->text(), QSql::In);
        query4.bindValue(":MOTY4", ui->lineEditMOTY4_2->text(), QSql::In);

        if(query4.exec()) {
            int ret = msgBox.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qDebug() << "Error: Data not added!";
        }
    }
    db.close();
}

void MainWindow::on_pushButtonMemCCPayTx_clicked()
{
    int bkRef;
    int acctRef;
    int txRef;
    QString txDate;
    QString desc;
    QString memo;
    QString amount;
    QMessageBox msgBox;


    msgBox.setText("Data Added!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug() << "Database: connection ok";
        QString queryTableCreateString = "CREATE IF NOT EXISTS TABLE MemTrans (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                         "bkRef INTEGER, acctRef INTEGER, txRef INTEGER, txDate VARCHAR(8), desc VARCHAR(30), memo VARCHAR(30), amount VARCHAR(15)";
        QString queryFindMaxTxRef = "SELECT MAX(txRef) FROM MemTrans;";
        QString queryInsertDataString = "INSERT INTO MemTrans (bkRef, acctRef, txRef, txDate, desc, memo, amount, DOTM, MOTY, MOTY2, MOTY3, MOTY4) \
                                        VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount, :DOTM, :MOTY, :MOTY2, :MOTY3, :MOTY4)";

        QSqlQuery query2(queryFindMaxTxRef);
        while (query2.next()){
        QString maxTxRef = query2.value(0).toString();
        int maxTxRefInt = maxTxRef.toInt();
        int nextTxRefInt = maxTxRefInt +1;
        QString nextTxRef = QString::number(nextTxRefInt);

        ui->lineEditTxRef_3->setText(nextTxRef);
        ui->lineEditTxRef_3->displayText();
        }

        QSqlQuery query3(queryTableCreateString);
        if(query3.exec())
            qDebug() << "Table Added";
        else {
            qDebug() << "Table Wasn't Added";
        }

        QSqlQuery query4;

        QString currentBank = ui->comboBoxBkRef_3->currentText();
        QString currentBankSliced = currentBank.sliced(0,2);

        QString currentAccount = ui->comboBoxCCAcct_2->currentText();
        QString currentAccountSliced = currentAccount.sliced(0,2);

        query4.prepare(queryInsertDataString);
        query4.bindValue(":bkRef", currentBankSliced.toInt(), QSql::In);
        query4.bindValue(":acctRef", currentAccountSliced.toInt(), QSql::In);
        query4.bindValue(":txRef", ui->lineEditTxRef_3->text().toInt(), QSql::In);
        query4.bindValue(":txDate", ui->lineEditDate_3->text(), QSql::In);
        query4.bindValue(":desc", ui->lineEditDesc_3->text(), QSql::In);
        query4.bindValue(":memo", ui->lineEditMemo_3->text(), QSql::In);
        query4.bindValue(":amount", ui->lineEditAmount_3->text(), QSql::In);
        query4.bindValue(":DOTM", ui->lineEditDOTM_3->text(), QSql::In);
        query4.bindValue(":MOTY", ui->lineEditMOTY_3->text(), QSql::In);
        query4.bindValue(":MOTY2", ui->lineEditMOTY2_3->text(), QSql::In);
        query4.bindValue(":MOTY3", ui->lineEditMOTY3_3->text(), QSql::In);
        query4.bindValue(":MOTY4", ui->lineEditMOTY4_3->text(), QSql::In);

        if(query4.exec()) {
            int ret = msgBox.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qDebug() << "Error: Data not added!";
        }
    }
    db.close();
}

void MainWindow::on_pushButtonViewMemTx_clicked()
{
    ui->tabWidget->setCurrentIndex(13);
}

void MainWindow::on_pushButtonViewMemTx_2_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QSqlQuery query;

        query.prepare("SELECT * FROM MemTrans WHERE desc LIKE :desc");
        QVariant TextSearch = ui->lineEditSearchMemTx->text();
        query.bindValue(":desc", TextSearch, QSql::In);
        query.exec();

        if(!query.exec()){
            qInfo() << "Query didn't execute";
        }
        else {
        QSqlQueryModel* model = new QSqlQueryModel();
        model->setQuery(std::move(query));
        ui->tableViewMemTx->setModel(model);
        }
    }
    db.close();
}

void MainWindow::on_pushButtonViewBillsDue_clicked()
{
    ui->tabWidget->setCurrentIndex(14);
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{   
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QSqlQuery query;

        QDate currentDate = ui->calendarWidget->selectedDate();
        QString currentDateStr = currentDate.toString();
        int day = currentDateStr.sliced(8,2).toInt();

        query.prepare("SELECT desc, MOTY, MOTY2, MOTY3, MOTY4 FROM MemTrans WHERE DOTM LIKE :MOTY");

        query.bindValue(":MOTY", day, QSql::In);
        query.exec();

        if(!query.exec()){
            qInfo() << "Query didn't execute";
        }
        else {
        QSqlQueryModel* model = new QSqlQueryModel();
        model->setQuery(std::move(query));
        ui->tableViewBillsDue->setModel(model);
        }
    }
    db.close();
}

void MainWindow::on_pushButtonDeleteExp_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Transaction Deleted Successfully");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QSqlQuery* query = new QSqlQuery(db);

        query->prepare("DELETE FROM transactions WHERE txRef LIKE :txRef;");
        QVariant TxRefText = ui->lineEditTxRef->text();

        query->bindValue(":txRef", TxRefText, QSql::In);
        query->exec();
        if(!query->exec()){
            qInfo() << "Query didn't execute";
        }
        else {
            int ret = msgBox.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        delete query;
    }
    ui->comboBoxBkRef_5->clear();
    ui->comboBoxBkRef_2->clear();
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef->clear();
    ui->comboBoxCCAcct->clear();
    ui->comboBoxExpAcct->clear();
    ui->comboBoxBkRef_3->clear();
    ui->comboBoxCCAcct_2->clear();
    loadAccountData();
    loadBankData();
    loadCCAccountData();
    db.close();
}

void MainWindow::on_pushButtonDeleteCCTx_clicked()
{
    int id;
    QMessageBox msgBox1;
    QMessageBox msgBox2;
    QMessageBox msgBox3;

    msgBox1.setText("Transaction Line 3 Deleted Successfully");
    msgBox1.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox2.setText("Transaction Line 2 Deleted Successfully");
    msgBox2.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox3.setText("Transaction Line 1 Deleted Successfully");
    msgBox3.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        QString queryDelectCCTx = "DELETE FROM transactions WHERE id == :id;";
        qDebug() << "Database: connection ok";

        QSqlQuery query;
        QString idText = ui->lineEditMaxID->text();

        query.prepare(queryDelectCCTx);
        int firstTx = idText.toInt();
        query.bindValue(":id", firstTx, QSql::In);
        if(query.exec()) {
            int ret = msgBox1.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qInfo() << "Query didn't execute!";
        }

        QSqlQuery query1;
        query1.prepare(queryDelectCCTx);
        int secondTx = (idText.toInt())-1;
        query1.bindValue(":id", secondTx, QSql::In);
        if(query1.exec()) {
            int ret = msgBox2.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qInfo() << "Query1 didn't execute!";
        }

        QSqlQuery query2;
        query2.prepare(queryDelectCCTx);
        int thirdTx = (idText.toInt())-2;
        query2.bindValue(":id", thirdTx, QSql::In);
        if(query2.exec()){
            int ret = msgBox3.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        else {
            qInfo() << "Query2 didn't execute!";
        }
    }
    ui->comboBoxBkRef_5->clear();
    ui->comboBoxBkRef_2->clear();
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef->clear();
    ui->comboBoxCCAcct->clear();
    ui->comboBoxExpAcct->clear();
    ui->comboBoxBkRef_3->clear();
    ui->comboBoxCCAcct_2->clear();
    loadAccountData();
    loadBankData();
    loadCCAccountData();
    db.close();
}

void MainWindow::on_pushButtonDeletePayTx_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Transaction Deleted Successfully");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    } else {
        QString queryDelectPayTx = "DELETE FROM transactions WHERE txRef == :txRef;";
        qDebug() << "Database: connection ok";

        QSqlQuery query;

        query.prepare(queryDelectPayTx);
        int txRefInt = ui->lineEditTxRef_3->text().toInt();
        query.bindValue(":txRef", txRefInt, QSql::In);
        if(query.exec()) {
            int ret = msgBox.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
    }
    ui->comboBoxBkRef_5->clear();
    ui->comboBoxBkRef_2->clear();
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef->clear();
    ui->comboBoxCCAcct->clear();
    ui->comboBoxExpAcct->clear();
    ui->comboBoxBkRef_3->clear();
    ui->comboBoxCCAcct_2->clear();
    loadAccountData();
    loadBankData();
    loadCCAccountData();
    db.close();
}

void MainWindow::on_pushButtonResetAssets_clicked()
{
    ui->textEditBankList->clear();
    ui->textEditBankBalances->clear();
    ui->lineEditSumOfAssets->clear();
}

void MainWindow::on_pushButtonResetCC_clicked()
{
    ui->textEditBalances->clear();
    ui->textEditCCAcct->clear();
    ui->lineEditTotalCCBal->clear();
}

void MainWindow::on_pushButtonSearchAcct_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QSqlQuery query;

        query.prepare("SELECT * FROM transactions WHERE acctRef LIKE :acctRef;");
        QVariant acctSearch = ui->lineEditAccount->text();
        int acctSearchInt = acctSearch.toInt();

        query.bindValue(":acctRef", acctSearchInt, QSql::In);
        query.exec();

        if(!query.exec()){
            qInfo() << "Query didn't execute";
        }
        else {
            QSqlQueryModel* model = new QSqlQueryModel();
            model->setQuery(std::move(query));
            ui->tableViewTxByDate->setModel(model);
        }
    }
    db.close();
}

void MainWindow::on_pushButtonDeleteSelectTx_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QMessageBox msgBox;
        QMessageBox msgBox1;
        QMessageBox msgBox2;
        msgBox.setText("Bank Record Deleted!");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox1.setText("Expense Record Deleted!");
        msgBox1.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox2.setText("Offset Record Deleted!");
        msgBox2.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        int row;
        row = ui->tableViewTxByDate->currentIndex().row();
        int id;
        id = ui->tableViewTxByDate->model()->data(ui->tableViewTxByDate->model()->index(row, 0)).toInt();

        QSqlQuery query;

        if(ui->checkBoxCCAcct->isChecked()) {
            query.prepare("DELETE FROM transactions WHERE id = :id");
            query.bindValue(":id", id);
            if(query.exec()) {
                int ret = msgBox.exec();
                switch(ret) {
                case QMessageBox::Ok:
                        break;
                case QMessageBox::Cancel:
                        break;
                default:
                    break;
                }
            }
            query.bindValue(":id", id+1);
            if(query.exec()) {
                int ret = msgBox1.exec();
                switch(ret) {
                case QMessageBox::Ok:
                        break;
                case QMessageBox::Cancel:
                        break;
                default:
                    break;
                }
            }
            query.bindValue(":id", id+2);
            if(query.exec()) {
                int ret = msgBox2.exec();
                switch(ret) {
                case QMessageBox::Ok:
                        break;
                case QMessageBox::Cancel:
                        break;
                default:
                    break;
                }
            }
        }
        else {
            query.prepare("DELETE FROM transactions WHERE id = :id");
            query.bindValue(":id", id);

            if(query.exec()) {
                int ret = msgBox.exec();
                switch(ret) {
                case QMessageBox::Ok:
                        break;
                case QMessageBox::Cancel:
                        break;
                default:
                    break;
                }
            }
        }
    }
    ui->comboBoxBkRef_5->clear();
    ui->comboBoxBkRef_2->clear();
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef->clear();
    ui->comboBoxCCAcct->clear();
    ui->comboBoxExpAcct->clear();
    ui->comboBoxBkRef_3->clear();
    ui->comboBoxCCAcct_2->clear();
    loadAccountData();
    loadBankData();
    loadCCAccountData();
    db.close();
}

void MainWindow::on_pushButtonSearchAmt_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QSqlQuery query;

        query.prepare("SELECT * FROM transactions WHERE amount LIKE :amount;");
        QVariant amtSearch = ui->lineEditAmount_5->text();

        query.bindValue(":amount", amtSearch, QSql::In);
        query.exec();

        if(!query.exec()){
            qInfo() << "Query didn't execute";
        }
        else {
            QSqlQueryModel* model = new QSqlQueryModel();
            model->setQuery(std::move(query));
            ui->tableViewTxByDate->setModel(model);
        }
    }
    db.close();
}

void MainWindow::on_pushButtonDelectMemTx_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Memorized Trans Record Deleted!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        int row;
        row = ui->tableViewBillsDue->currentIndex().row();
        QString desc;
        desc = ui->tableViewBillsDue->model()->data(ui->tableViewBillsDue->model()->index(row, 0)).toString();

        QSqlQuery query;

        query.prepare("DELETE FROM MemTrans WHERE desc = :desc");
        query.bindValue(":desc", desc);

        if(query.exec()) {
            int ret = msgBox.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
    }
    db.close();
}

void MainWindow::on_pushButtonViewExp_clicked()
{
    ui->tabWidget->setCurrentIndex(15);
}

void MainWindow::on_pushButtonViewExp_2_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QSqlQuery query;
        QString month = ui->lineEditMonth->text();

        query.prepare("SELECT accounts.name AS Account, sum(transactions.amount) AS Spent, accounts.budget AS Budgeted \
                      FROM accounts\
                      INNER JOIN transactions ON accounts.acctRef = transactions.acctRef \
                      WHERE transactions.amount > 0 AND accounts.acctRef != 0 AND accounts.name NOT LIKE '%-Bank Adjustment' \
                      AND accounts.name NOT LIKE '%-%-%' AND transactions.txDate LIKE :txDate\
                      GROUP BY accounts.bkRef, accounts.acctRef;");

        query.bindValue(":txDate", month, QSql::In);

        if(!query.exec()){
            qInfo() << "Query didn't execute";
        }
        else {
        QSqlQueryModel* model = new QSqlQueryModel();
        model->setQuery(std::move(query));
        ui->tableViewExp->setModel(model);
        }
    }
    db.close();
}

void MainWindow::on_pushButtonDeleteBank_clicked()
{
    QMessageBox msgBox;
    QMessageBox msgBox1;
    QMessageBox msgBox2;
    QMessageBox msgBox3;
    QMessageBox msgBox4;
    QMessageBox msgBox5;
    msgBox.setText("Bank Deleted Successfully");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox1.setText("Transaction Deleted Successfully");
    msgBox1.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox2.setText("Warning! Are you sure?");
    msgBox2.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox3.setText("Bank account deletion is Cancelled");
    msgBox3.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox4.setText("Transaction deletion is Cancelled");
    msgBox4.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox5.setText("Asset Deletion is Cancelled");
    msgBox5.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if(ui->checkBoxDeleteOk->isChecked()) {
        int ret = msgBox2.exec();
        switch(ret) {
        case QMessageBox::Ok:
            break;
        case QMessageBox::Cancel:
            ui->checkBoxDeleteOk->setCheckState(Qt::CheckState(0));
            break;
        default:
            break;
        }
        if(ui->checkBoxDeleteOk->isChecked()) {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
            db.open();
            if (!db.open()) {
                qDebug() << "Error: connection with database failed";
            }
            else {
                QSqlQuery* query = new QSqlQuery(db);

                query->prepare("DELETE FROM banks WHERE bkRef LIKE :bkRef;");
                int bkRefTextInt = ui->lineEditBkRef->text().toInt();

                query->bindValue(":bkRef", bkRefTextInt, QSql::In);
                query->exec();
                if(!query->exec()){
                    qInfo() << "Query didn't execute";
                }
                else {
                    if(ui->checkBoxDeleteOk->isChecked()) {
                        int ret = msgBox.exec();
                        switch(ret) {
                        case QMessageBox::Ok:
                                break;
                        case QMessageBox::Cancel:
                                break;
                        default:
                            break;
                        }
                    }
                    else {
                        int ret = msgBox3.exec();
                        switch(ret) {
                        case QMessageBox::Ok:
                            break;
                        case QMessageBox::Cancel:
                            break;
                        default:
                            break;
                        }
                    }
                    if(ui->checkBoxDeleteOk->isChecked()) {
                        delete query;

                        QSqlQuery* query1 = new QSqlQuery(db);

                        query1->prepare("DELETE FROM transactions WHERE bkRef LIKE :bkRef;");
                        int bkRefText1Int = ui->lineEditBkRef->text().toInt();

                        query1->bindValue(":bkRef", bkRefText1Int, QSql::In);
                        query1->exec();
                        if(!query1->exec()){
                            qInfo() << "Query didn't execute";
                        }
                        else {
                            int ret = msgBox1.exec();
                            switch(ret) {
                            case QMessageBox::Ok:
                                    break;
                            case QMessageBox::Cancel:
                                    break;
                            default:
                                break;
                            }
                        }
                        delete query1;
                    }
                    else {
                        int ret = msgBox4.exec();
                        switch(ret) {
                        case QMessageBox::Ok:
                            break;
                        case QMessageBox::Cancel:
                            break;
                        default:
                            break;
                        }
                    }
                }
                ui->checkBoxDeleteOk->setCheckState(Qt::CheckState(0));
                ui->comboBoxBkRef_5->clear();
                ui->comboBoxBkRef_2->clear();
                ui->comboBoxAcctRef_2->clear();
                ui->comboBoxBkRef->clear();
                ui->comboBoxCCAcct->clear();
                ui->comboBoxExpAcct->clear();
                ui->comboBoxBkRef_3->clear();
                ui->comboBoxCCAcct_2->clear();
                loadAccountData();
                loadBankData();
                loadCCAccountData();
                db.close();
            }
        }
        else {
            int ret = msgBox5.exec();
            switch(ret) {
            case QMessageBox::Ok:
                break;
            case QMessageBox::Cancel:
                ui->checkBoxDeleteOk->setCheckState(Qt::CheckState(0));
                break;
            default:
                break;
            }
        }
    }
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButtonDeleteAccount_clicked()
{
    QMessageBox msgBox;
    QMessageBox msgBox1;
    msgBox.setText("Account Deleted Successfully");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox1.setText("Transaction Deleted Successfully");
    msgBox1.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QSqlQuery* query = new QSqlQuery(db);

        query->prepare("DELETE FROM accounts WHERE acctRef LIKE :acctRef;");
        QVariant AcctRefText = ui->lineEditAcctRef->text();

        query->bindValue(":acctRef", AcctRefText, QSql::In);
        query->exec();
        if(!query->exec()){
            qInfo() << "Query didn't execute";
        }
        else {
            int ret = msgBox.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        delete query;

        QSqlQuery* query1 = new QSqlQuery(db);

        query1->prepare("DELETE FROM transactions WHERE acctRef LIKE :acctRef;");
        int acctRefText1Int = ui->lineEditAcctRef->text().toInt();

        query1->bindValue(":acctRef", acctRefText1Int, QSql::In);
        query1->exec();
        if(!query1->exec()){
            qInfo() << "Query didn't execute";
        }
        else {
            int ret = msgBox1.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        delete query1;
    }
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef_5->clear();
    ui->comboBoxBkRef_2->clear();
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef->clear();
    ui->comboBoxCCAcct->clear();
    ui->comboBoxExpAcct->clear();
    ui->comboBoxBkRef_3->clear();
    ui->comboBoxCCAcct_2->clear();
    loadAccountData();
    loadBankData();
    loadCCAccountData();
    db.close();
}

void MainWindow::on_pushButtonDeleteCreditAccount_clicked()
{
    QMessageBox msgBox;
    QMessageBox msgBox1;
    msgBox.setText("Credit Account Deleted Successfully");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox1.setText("Transaction Deleted Successfully");
    msgBox1.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QSqlQuery* query = new QSqlQuery(db);

        query->prepare("DELETE FROM cc_account WHERE ccRef LIKE :ccRef;");
        QVariant ccRefText = ui->lineEditCreditAcct->text();

        query->bindValue(":ccRef", ccRefText, QSql::In);
        query->exec();
        if(!query->exec()){
            qInfo() << "Query didn't execute";
        }
        else {
            int ret = msgBox.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        delete query;

        QSqlQuery* query1 = new QSqlQuery(db);

        query1->prepare("DELETE FROM transactions WHERE acctRef LIKE :acctRef;");
        int ccRefText1Int = ui->lineEditCreditAcct->text().toInt();

        query1->bindValue(":acctRef", ccRefText1Int, QSql::In);
        query1->exec();
        if(!query1->exec()){
            qInfo() << "Query didn't execute";
        }
        else {
            int ret = msgBox1.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
        delete query1;
    }
    ui->comboBoxCCAcct_2->clear();
    ui->comboBoxBkRef_5->clear();
    ui->comboBoxBkRef_2->clear();
    ui->comboBoxAcctRef_2->clear();
    ui->comboBoxBkRef->clear();
    ui->comboBoxCCAcct->clear();
    ui->comboBoxExpAcct->clear();
    ui->comboBoxBkRef_3->clear();
    ui->comboBoxCCAcct_2->clear();
    loadAccountData();
    loadBankData();
    loadCCAccountData();
    db.close();
}

void MainWindow::on_pushButtonDeleteMemTx_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("MemTx Record Deleted!");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        int row;
        row = ui->tableViewMemTx->currentIndex().row();
        int id;
        id = ui->tableViewMemTx->model()->data(ui->tableViewMemTx->model()->index(row, 0)).toInt();

        QSqlQuery query;

        query.prepare("DELETE FROM MemTrans WHERE id = :id");
        query.bindValue(":id", id);
        if(query.exec()) {
            int ret = msgBox.exec();
            switch(ret) {
            case QMessageBox::Ok:
                    break;
            case QMessageBox::Cancel:
                    break;
            default:
                break;
            }
        }
    }
    db.close();
}

void MainWindow::on_pushButtonModifyBudget_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Account Record Updated!");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        int newBudget = ui->lineEditNewBudget->text().toInt();
        int row;
        row = ui->tableViewExp->currentIndex().row();
        QVariant nameInput = ui->tableViewExp->model()->data(ui->tableViewExp->model()->index(row, 0)).toString();

        QSqlQuery query;

        query.prepare("UPDATE accounts SET budget = :newBudget WHERE name = :name");
        query.bindValue(":name", nameInput, QSql::In);
        query.bindValue(":newBudget", newBudget, QSql::In);

        if(!query.exec()){
            qInfo() << "Query didn't execute";
        }
        else {
        on_pushButtonViewExp_2_clicked();
        int ret = msgBox.exec();
        switch(ret) {
        case QMessageBox::Ok:
                break;
        case QMessageBox::Cancel:
                break;
        default:
            break;
        }
    }
    db.close();
    }
}

void MainWindow::on_pushButtonViewTotals_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QSqlQuery query;

        query.prepare("SELECT sum(accounts.budget) FROM accounts WHERE budget > 0 AND name NOT LIKE '%-%-%';");
        if(!query.exec()){
            qInfo() << "Query didn't execute";
        }
        else {
            query.next();
            ui->lineEditTotalBudgeted->setText(query.value(0).toString());

            int sumOfSpent = 0;
            int sumOfBudgeted = 0;
            QAbstractItemModel *model = ui->tableViewExp->model();

            for(int row = 0; row < model->rowCount(); ++row){
                QModelIndex index = model->index(row, 1);
                int value = model->data(index).toInt();
                if(value > 0){
                sumOfSpent += value;
                }
                else{
                    continue;
                }
            }
            ui->lineEditTotalSpent->setText(QString::number(sumOfSpent));

            for(int row = 0; row < model->rowCount(); ++row){
                QModelIndex index = model->index(row, 2);
                int value = model->data(index).toInt();
                if(value > 0){
                sumOfBudgeted += value;
                }
                else{
                    continue;
                }
            }
            ui->lineEditTotalBudgeted->setText(QString::number(sumOfBudgeted));
        }      
    }
    db.close();
}

void MainWindow::on_pushButtonSetBudgets_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Budgets Updated Successfully!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        int spent;
        QString name;
        QModelIndex index1, index2;
        QSqlQuery query;
        QAbstractItemModel *model = ui->tableViewExp->model();

        for(int row = 0; row < model->rowCount(); ++row){
            index1 = model->index(row, 0);
            name = model->data(index1).toString();
            index2 = model->index(row, 1);
            spent = model->data(index2).toInt();
            query.prepare("UPDATE accounts SET budget = :spent WHERE name = :name");
            query.bindValue(":name", name, QSql::In);
            query.bindValue(":spent", spent, QSql::In);

            if(!query.exec()){
                qInfo() << "Query didn't execute";
            }
            else {
                continue;
            }
        }
        int ret = msgBox.exec();
        switch(ret) {
        case QMessageBox::Ok:
                break;
        case QMessageBox::Cancel:
                break;
        default:
            break;
        }
        on_pushButtonViewExp_2_clicked();
    }
    db.close();
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    QMessageBox msgBox;
    msgBox.setText("Bank State Changed Successfully");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if(checked) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
        db.open();
        if (!db.open()) {
            qDebug() << "Error: connection with database failed";
        }
        else {
            QSqlQuery* query = new QSqlQuery(db);

            query->prepare("UPDATE banks SET active = 'false' WHERE bkRef LIKE :bkRef;");
            int bkRefTextInt = ui->lineEditBkRef->text().toInt();

            query->bindValue(":bkRef", bkRefTextInt, QSql::In);
            query->exec();
            if(!query->exec()){
                qInfo() << "Query didn't execute";
            }
            else {
                ui->lineEditStatus->setText("Inactive");
                int ret = msgBox.exec();
                switch(ret) {
                case QMessageBox::Ok:
                        break;
                case QMessageBox::Cancel:
                        break;
                default:
                    break;
                }
            }
            delete query;
        }
    }
    if(!checked) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
        db.open();
        if (!db.open()) {
            qDebug() << "Error: connection with database failed";
        }
        else {
            QSqlQuery* query = new QSqlQuery(db);

            query->prepare("UPDATE banks SET active = 'true' WHERE bkRef LIKE :bkRef;");
            int bkRefTextInt = ui->lineEditBkRef->text().toInt();

            query->bindValue(":bkRef", bkRefTextInt, QSql::In);
            query->exec();
            if(!query->exec()){
                qInfo() << "Query didn't execute";
            }
            else {
                ui->lineEditStatus->setText("Active");
                int ret = msgBox.exec();
                switch(ret) {
                case QMessageBox::Ok:
                        break;
                case QMessageBox::Cancel:
                        break;
                default:
                    break;
                }
            }
            delete query;
        }
    }
}

void MainWindow::on_pushButtonNewFiscalYear_clicked()
{
    QMessageBox msgBox;
    QMessageBox msgBox1;
    QMessageBox msgBox2;
    msgBox.setText("Table Data Deleted Successfully");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox1.setText("Warning! Are you sure?");
    msgBox1.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox2.setText("New Fiscal Year Migration is cancelled");
    msgBox2.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if(ui->checkBoxDeleteAllOk->isChecked()) {
        int ret = msgBox1.exec();
        switch(ret) {
        case QMessageBox::Ok:
            break;
        case QMessageBox::Cancel:
            ui->checkBoxDeleteAllOk->setCheckState(Qt::CheckState(0));
            break;
        default:
            break;
        }
        if(ui->checkBoxDeleteAllOk->isChecked()) {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
            db.open();
            if (!db.open()) {
                qDebug() << "Error: connection with database failed";
            }
            else {
                QSqlQuery query1;
                
                query1.prepare("DELETE FROM transactions;");
                query1.exec();
                if(!query1.exec()){
                    qInfo() << "Transactions data not deleted";
                    query1.lastError();
                }
                int ret = msgBox.exec();
                switch(ret) {
                case QMessageBox::Ok:
                        break;
                case QMessageBox::Cancel:
                        break;
                default:
                    break;
                }
                ui->comboBoxBkRef_5->clear();
                ui->comboBoxBkRef_2->clear();
                ui->comboBoxAcctRef_2->clear();
                ui->comboBoxBkRef->clear();
                ui->comboBoxCCAcct->clear();
                ui->comboBoxExpAcct->clear();
                ui->comboBoxBkRef_3->clear();
                ui->comboBoxCCAcct_2->clear();
                ui->checkBoxDeleteAllOk->setCheckState(Qt::CheckState(0));
            }
            db.close();
        }
        else {
            int ret = msgBox2.exec();
            switch(ret) {
            case QMessageBox::Ok:
                break;
            case QMessageBox::Cancel:
                break;
            default:
                break;
            }
            ui->tabWidget->setCurrentIndex(0);
        }
    }
}



