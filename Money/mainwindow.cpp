#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlResult>
#include <QAbstractItemModel>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlError>
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
            ui->comboBoxAssetSelect->addItem(bankEntry);
            ui->comboBoxReconAcct->addItem(bankEntry);
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
            ui->comboBoxAcctSelect->addItem(accountEntry);
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
        QString queryInsertDataString = "INSERT INTO transactions (bkRef, acctRef, txRef, txDate, desc, memo, amount, fmt_txDate) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount, :fmt_txDate)";

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

        QSqlQuery query4(queryInsertDataString);

        QString currentBank = ui->comboBoxBkRef_2->currentText();
        QString currentBankSliced = currentBank.sliced(0,2);

        QString currentAccount = ui->comboBoxAcctRef_2->currentText();
        QString currentAccountSliced = currentAccount.sliced(0,2);
        QString transDate = ui->lineEditDate->text();
        QString fmtTransDate = transDate.sliced(4, 4) + transDate.sliced(0, 4);

        query4.prepare(queryInsertDataString);
        query4.bindValue(":bkRef", currentBankSliced.toInt(), QSql::In);
        query4.bindValue(":acctRef", currentAccountSliced.toInt(), QSql::In);
        query4.bindValue(":txRef", ui->lineEditTxRef->text().toInt(), QSql::In);
        query4.bindValue(":txDate", ui->lineEditDate->text(), QSql::In);
        query4.bindValue(":desc", ui->lineEditDesc->text(), QSql::In);
        query4.bindValue(":memo", ui->lineEditMemo->text(), QSql::In);
        query4.bindValue(":amount", ui->lineEditAmount->text(), QSql::In);
        query4.bindValue(":fmt_txDate", fmtTransDate, QSql::In);

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
    if(ui->checkBox_Split_Exp->isChecked()){
        ui->spinBox_Split_Exp->setValue(ui->spinBox_Split_Exp->value()-1);
        int splitAmount = ui->lineEdit_Split_Exp->text().toInt();
        int transAmount = ui->lineEditAmount->text().toInt();
        int splitBalance = splitAmount-transAmount;
        QString splitBalanceStr = QString::number(splitBalance);
        ui->lineEdit_Split_Exp->setText(splitBalanceStr);
    }
    else{
        db.close();
    }
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
        QString queryInsertDataString1 = "INSERT INTO transactions (bkRef, acctRef, txRef, txDate, desc, memo, amount, fmt_txDate) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount, :fmt_txDate)";
        QString queryInsertDataString2 = "INSERT INTO transactions (bkRef, acctRef, txRef, txDate, desc, memo, amount, fmt_txDate) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount, :fmt_txDate)";
        QString queryInsertDataString3 = "INSERT INTO transactions (bkRef, acctRef, txRef, txDate, desc, memo, amount, fmt_txDate) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount, :fmt_txDate)";
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
        QString transDate = ui->lineEditDate_2->text();
        QString fmtTransDate = transDate.sliced(4, 4) + transDate.sliced(0, 4);

        QSqlQuery query4(queryInsertDataString1);
        query4.prepare(queryInsertDataString1);
        query4.bindValue(":bkRef", currentBankSliced.toInt(), QSql::In);
        query4.bindValue(":acctRef", creditCardAccountSliced.toInt(), QSql::In);
        query4.bindValue(":txRef", ui->lineEditTxRef->text().toInt(), QSql::In);
        query4.bindValue(":txDate", ui->lineEditDate_2->text(), QSql::In);
        query4.bindValue(":desc", ui->lineEditDesc_2->text(), QSql::In);
        query4.bindValue(":memo", ui->lineEditMemo_2->text(), QSql::In);
        query4.bindValue(":amount", ui->lineEditAmount_2->text(), QSql::In);
        query4.bindValue(":fmt_txDate", fmtTransDate, QSql::In);
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
        query5.bindValue(":fmt_txDate", fmtTransDate, QSql::In);
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
        query6.bindValue(":txDate", ui->lineEditDate_2->text(), QSql::In);
        query6.bindValue(":desc", "0", QSql::In);
        query6.bindValue(":memo", "0", QSql::In);
        query6.bindValue(":amount", offSetString, QSql::In);
        query6.bindValue(":fmt_txDate", fmtTransDate, QSql::In);

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
    if(ui->checkBox_Split_CC->isChecked()){
        ui->spinBox_Split_CC->setValue(ui->spinBox_Split_CC->value()-1);
        int splitAmount = ui->lineEdit_Split_CC->text().toInt();
        int transAmount = ui->lineEditAmount_2->text().toInt();
        int splitBalance = splitAmount-transAmount;
        QString splitBalanceStr = QString::number(splitBalance);
        ui->lineEdit_Split_CC->setText(splitBalanceStr);
    }
    else{
        db.close();
    }
}

void MainWindow::on_pushButtonDispAssets_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else
    {
        QString queryBanks = "Select bkRef FROM banks WHERE active == 'true';";
        QString queryBankBalances =
            "WITH date_Query AS (Select * FROM transactions WHERE fmt_txDate <= strftime('%Y%m%d', date('now')) AND (bkRef == 1 OR bkRef == 2 OR bkRef == 3 "
            "OR bkRef == 4 OR bkRef == 5 OR bkRef == 7 OR bkRef == 8 OR bkRef == 9 OR bkRef == 10 OR bkRef == 11)) \
            SELECT bkRef, SUM(amount) FROM date_Query \
            WHERE bkRef LIKE :bkRef \
            UNION \
            SELECT bkRef, name FROM banks \
            WHERE bkRef LIKE :bkRef;";

        QSqlQuery query1(queryBanks);
        QSqlQuery query3(queryBankBalances);

        int sum = 0;
        int bankBalanceInt;
        while(query1.next()){

        query3.prepare(queryBankBalances);
        query3.bindValue(":bkRef", query1.value(0), QSql::InOut);

        if(!query3.exec()){
                qInfo() << "BankBalances query didn't execute!";
                qInfo() << query3.lastError();
        }
        else{
                qInfo() << "BankBalances query executed";
                query3.next();
                QString bankAccount = query3.value(0).toString();
                QString bankBalance = query3.value(1).toString();

                bankBalanceInt = bankBalance.toInt();
                sum += bankBalanceInt;
                query3.next();

                QString name = query3.value(1).toString();
                QString result = bankAccount + " " + name;
                ui->textEditBankList->append(result);

                if(bankBalanceInt !=0) {
                    double bankBalanceDouble = bankBalanceInt/100;
                    QString bankBalanceDoubleString = QString::number(bankBalanceDouble);

                    QString bankBalanceDecimalString = bankBalance.sliced((bankBalance.length()-2), 2);
                    QString balance = '$' + bankBalanceDoubleString + '.' + bankBalanceDecimalString;
                    ui->textEditBankBalances->append(balance);
                }
                else{
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
                else{
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
        QString queryInsertDataString = "INSERT INTO transactions (bkRef, acctRef, txRef, txDate, desc, memo, amount, fmt_txDate) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount, :fmt_txDate)";
        QString queryInsertDataString1 = "INSERT INTO transactions (bkRef, acctRef, txRef, txDate, desc, memo, amount, fmt_txDate) VALUES (:bkRef, :acctRef, :txRef, :txDate, :desc, :memo, :amount, :fmt_txDate)";


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
        QString transDate = ui->lineEditDate_3->text();
        QString fmtTransDate = transDate.sliced(4, 4) + transDate.sliced(0, 4);

        query4.prepare(queryInsertDataString);
        query4.bindValue(":bkRef", currentBankSliced.toInt(), QSql::In);
        query4.bindValue(":acctRef", currentCCAccountSliced.toInt(), QSql::In);
        query4.bindValue(":txRef", ui->lineEditTxRef_3->text().toInt(), QSql::In);
        query4.bindValue(":txDate", ui->lineEditDate_3->text(), QSql::In);
        query4.bindValue(":desc", ui->lineEditDesc_3->text(), QSql::In);
        query4.bindValue(":memo", ui->lineEditMemo_3->text(), QSql::In);
        query4.bindValue(":amount", CCOffsetAmountString, QSql::In);
        query4.bindValue(":fmt_txDate", fmtTransDate, QSql::In);

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
        query5.bindValue(":fmt_txDate", fmtTransDate, QSql::In);

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
        QString query_ccAcct = "SELECT ccRef, desc FROM cc_account;";
        QString queryAccountBalances =
            "WITH query_CCards AS (SELECT * FROM transactions "
            "WHERE fmt_txDate <= strftime('%Y%m%d', date('now')) AND (acctRef == 51 OR acctRef == 52 OR acctRef == 53 OR acctRef == 69 "
            "OR acctRef == 72 OR acctRef == 76 OR acctRef == 81 OR acctRef == 91)) \
            SELECT acctRef, SUM(amount) FROM query_CCards \
            WHERE acctRef LIKE :acctRef \
            UNION \
            SELECT ccRef, desc FROM cc_account \
            WHERE ccRef LIKE :ccRef;";

        QSqlQuery query1(query_ccAcct);
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

                QString name = query1.value(1).toString();
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

        if (ui->checkBoxAddTxRef->isChecked()) {
            query.prepare("SELECT * \
                           FROM transactions \
                           WHERE txDate LIKE :txDate \
                           Order By fmt_txDate DESC;");
        }
        else {
            query.prepare("SELECT txDate, acctRef, desc, memo, amount, fmt_txDate \
                           FROM transactions \
                           WHERE txDate LIKE :txDate AND txRef != 0 \
                           Order By fmt_txDate DESC;");
        }

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

        if (ui->checkBoxAddTxRef->isChecked()) {
            query.prepare("SELECT * \
                           FROM transactions \
                           WHERE desc LIKE :desc \
                           Order By fmt_txDate DESC;");
        }
        else {
            query.prepare("SELECT fmt_txDate, acctRef, desc, memo, amount \
                           FROM transactions \
                           WHERE desc LIKE :desc AND txRef != 0 \
                           Order By fmt_txDate DESC;");
        }

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
        QString queryInsertDataString = "INSERT INTO MemTrans (bkRef, acctRef, expRef, txRef, txDate, desc, memo, amount, DOTM, MOTY, MOTY2, MOTY3, MOTY4) \
                                        VALUES (:bkRef, :acctRef, :expRef, :txRef, :txDate, :desc, :memo, :amount, :DOTM, :MOTY, :MOTY2, :MOTY3, :MOTY4)";

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
        query4.bindValue(":expRef", currentExpAccountSliced.toInt(), QSql::In);
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

        if (ui->checkBoxAddTxRef->isChecked()) {
            query.prepare("SELECT * \
                           FROM transactions \
                           WHERE acctRef LIKE :acctRef \
                           Order By fmt_txDate DESC;");
        }
        else {
            query.prepare("SELECT fmt_txDate, acctRef, desc, memo, amount \
                           FROM transactions \
                           WHERE acctRef LIKE :acctRef AND txRef != 0 \
                           Order By fmt_txDate DESC;");
        }

        QString acctSearch = ui->comboBoxAcctSelect->currentText();
        QString acctSearchSliced = acctSearch.sliced(0,2);
        int acctSearchInt = acctSearchSliced.toInt();

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

        if (ui->checkBoxAddTxRef->isChecked()) {
            query.prepare("SELECT * \
                           FROM transactions \
                           WHERE amount LIKE :amount \
                           Order By fmt_txDate DESC;");
        }
        else {
            query.prepare("SELECT fmt_txDate, acctRef, desc, memo, amount \
                           FROM transactions \
                           WHERE amount LIKE :amount AND txRef != 0 \
                           Order By fmt_txDate DESC;");
        }

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
        QSqlQuery query1;
        QString month = ui->lineEditMonth->text();

        query.prepare("SELECT accounts.name AS Account, sum(transactions.amount) AS Spent, accounts.budget AS Budgeted, \
                      (accounts.budget - sum(transactions.amount)) AS Diff \
                      FROM accounts \
                      INNER JOIN transactions ON accounts.acctRef = transactions.acctRef \
                      WHERE transactions.amount > 0 AND accounts.acctRef != 0 AND accounts.name NOT LIKE '%-Bank Adjustment' \
                      AND accounts.name NOT LIKE '%-%-%' AND transactions.txDate LIKE :txDate \
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

        query1.prepare("SELECT acctRef, txDate, amount FROM transactions \
                       WHERE (acctRef == 59 OR acctRef == 62 OR acctRef == 63) AND txDate LIKE :txDate;");

        query1.bindValue(":txDate", month, QSql::In);

        if(!query1.exec()){
        qInfo() << "Query didn't execute";
        }
        else {
            query1.next();

            QSqlQueryModel* model2 = new QSqlQueryModel();
            model2->setQuery(std::move(query1));
            ui->tableView_Income->setModel(model2);

            int sumOfIncome = 0;
            QAbstractItemModel* model3 = ui->tableView_Income->model();

           for(int row = 0; row < model3->rowCount(); ++row){
               QModelIndex index = model3->index(row, 2);
               int value = model3->data(index).toInt();
               if(value != 0){
                    sumOfIncome += value;
                }
                else {
                    continue;
                }
           }
        ui->lineEdit_MonthIncome->setText(QString::number(sumOfIncome));
        }
    }
    db.close();
}

void MainWindow::on_pushButtonViewExp_3_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QSqlQuery query;
        QString monthText = ui->lineEditMonth->text().sliced(0,2);
        QString yearText = ui->lineEditMonth->text().sliced(3,4);
        int monthInt = monthText.toInt();
        int yearInt = yearText.toInt();
        int priorMonth = monthInt - 1;
        QString priorMonthText = QString::number(priorMonth);
        int priorYear = yearInt - 1;
        QString priorYearText = QString::number(priorYear);

        if (monthText == "01") {
            priorMonthText = "12";
            QString priorDate = priorMonthText + '%' + priorYearText;
            query.prepare("SELECT accounts.name AS Account, sum(transactions.amount) AS Spent \
                          FROM accounts \
                          INNER JOIN transactions ON accounts.acctRef = transactions.acctRef \
                          WHERE transactions.amount > 0 AND accounts.acctRef != 0 AND accounts.name NOT LIKE '%-Bank Adjustment' \
                          AND accounts.name NOT LIKE '%-%-%' AND transactions.txDate LIKE :txDate \
                          GROUP BY accounts.bkRef, accounts.acctRef;");

                          query.bindValue(":txDate", priorDate, QSql::In);
        }

        if (monthInt == 12 || monthInt == 11) {
            QString priorDate = priorMonthText + '%' + yearText;
            query.prepare("SELECT accounts.name AS Account, sum(transactions.amount) AS Spent \
                          FROM accounts \
                          INNER JOIN transactions ON accounts.acctRef = transactions.acctRef \
                          WHERE transactions.amount > 0 AND accounts.acctRef != 0 AND accounts.name NOT LIKE '%-Bank Adjustment' \
                          AND accounts.name NOT LIKE '%-%-%' AND transactions.txDate LIKE :txDate \
                          GROUP BY accounts.bkRef, accounts.acctRef;");

                          query.bindValue(":txDate", priorDate, QSql::In);
        }

        if (monthInt <= 10 && monthInt > 1) {
            QString priorMonthText = '0' + QString::number(priorMonth);
            QString priorDate = priorMonthText + '%' + yearText;
            query.prepare("SELECT accounts.name AS Account, sum(transactions.amount) AS Spent \
                          FROM accounts \
                          INNER JOIN transactions ON accounts.acctRef = transactions.acctRef \
                          WHERE transactions.amount > 0 AND accounts.acctRef != 0 AND accounts.name NOT LIKE '%-Bank Adjustment' \
                          AND accounts.name NOT LIKE '%-%-%' AND transactions.txDate LIKE :txDate \
                          GROUP BY accounts.bkRef, accounts.acctRef;");

                          query.bindValue(":txDate", priorDate, QSql::In);
        }

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

            int sumOfSpent = 0;
            int sumOfBudgeted = 0;
            int sumOfOverBudget = 0;
            QAbstractItemModel *model = ui->tableViewExp->model();

            for(int row = 0; row < model->rowCount(); ++row){
                QModelIndex index = model->index(row, 1);
                int value = model->data(index).toInt();
                if(value != 0){
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
                if(value != 0){
                    sumOfBudgeted += value;
                }
                else{
                    continue;
                }
            }
            ui->lineEdit_Budgeted->setText(QString::number(sumOfBudgeted));

            for(int row = 0; row < model->rowCount(); ++row){
                QModelIndex index = model->index(row, 3);
                int value = model->data(index).toInt();
                if(value != 0){
                sumOfOverBudget += value;
                }
                else{
                    continue;
                }
            }
            ui->lineEditTotalOverBudget->setText(QString::number(sumOfOverBudget));
        }      
    }
    db.close();
}

void MainWindow::on_pushButtonSetBudgets_clicked()
{
    if(ui->checkBox_Enable_Spent->isChecked()){
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

void MainWindow::on_pushButtonSearchAsset_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QSqlQuery query;

        if (ui->checkBoxAddTxRef->isChecked()) {
            query.prepare("SELECT * \
                           FROM transactions \
                           WHERE bkRef LIKE :bkRef \
                           Order By fmt_txDate DESC;");
        }
        else {
            query.prepare("SELECT fmt_txDate, acctRef, desc, memo, amount \
                           FROM transactions \
                           WHERE bkRef LIKE :bkRef AND txRef != 0 \
                           Order By fmt_txDate DESC;");
        }

        QString assetSearch = ui->comboBoxAssetSelect->currentText();
        QString assetSearchSliced = assetSearch.sliced(0,2);
        int assetSearchInt = assetSearchSliced.toInt();

        query.bindValue(":bkRef", assetSearchInt, QSql::In);
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


void MainWindow::on_pushButton_Mod_Date_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Transaction Record Updated!");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        QString newDateStr = ui->lineEdit_Mod_Date->text();
        int newDate = ui->lineEdit_Mod_Date->text().toInt();
        QString fmt_newDateStr = newDateStr.sliced(4, 4) + newDateStr.sliced(0, 4);

        QString updateTxDateStr =
            "UPDATE transactions SET txDate = :newDate WHERE id = :id;";
        QString updateFmt_TxDateStr =
            "UPDATE transactions SET fmt_txDate = :fmt_newDateStr WHERE id = :id;";

        int row;
        row = ui->tableViewTxByDate->currentIndex().row();
        QVariant idInput = ui->tableViewTxByDate->model()->data(ui->tableViewTxByDate->model()->index(row, 0)).toString();

        QSqlQuery query(updateTxDateStr);
        QSqlQuery query1(updateFmt_TxDateStr);

        query.prepare(updateTxDateStr);
        query.bindValue(":id", idInput, QSql::In);
        query.bindValue(":newDate", newDate, QSql::In);

        if(!query.exec()){
            qInfo() << "Update_txDate Query didn't execute";
            qInfo() << query.lastError();
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

        query1.prepare(updateFmt_TxDateStr);
        query1.bindValue(":id", idInput, QSql::In);
        query1.bindValue(":fmt_newDateStr", fmt_newDateStr, QSql::In);

        if(!query1.exec()){
            qInfo() << "Update fmt_txDate Query didn't execute";
            qInfo() << query1.lastError();
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
        ui->lineEdit_Mod_Date->clear();
        db.close();
    }
}


void MainWindow::on_pushButton_Mod_Amt_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Transaction Record Updated!");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        int newAmt = ui->lineEdit_Mod_Amt->text().toInt();
        int row;
        row = ui->tableViewTxByDate->currentIndex().row();
        QVariant idInput = ui->tableViewTxByDate->model()->data(ui->tableViewTxByDate->model()->index(row, 0)).toString();

        QSqlQuery query;

        query.prepare("UPDATE transactions SET amount = :amount WHERE id = :id");
        query.bindValue(":id", idInput, QSql::In);
        query.bindValue(":amount", newAmt, QSql::In);

        if(!query.exec()){
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
        ui->lineEdit_Mod_Amt->clear();
        db.close();
    }
}


void MainWindow::on_pushButtonReconAccounts_clicked()
{
    ui->tabWidget->setCurrentIndex(16);
}


void MainWindow::on_pbReconOK_clicked()
{
    ui->tabWidget->setCurrentIndex(17);
    ui->lineEditReconAccount->setText(ui->comboBoxReconAcct->currentText());
    ui->lineEditReconBegBal_2->setText(ui->lineEditReconBegBal->text());
    ui->lineEditReconTarBal->setText(ui->lineEditReconEndBal->text());
    on_pushButtonRefreshReconPayments_clicked();
    on_pushButtonRefreshReconPayments_clicked();
}

void MainWindow::on_pbReconCancel_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButtonRefreshReconPayments_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QVariant reconPayments = ui->comboBoxReconAcct->currentText();
        QString reconPaymentsStr = reconPayments.toString();
        QString reconStmtEndDate = ui->lineEditReconStmtEndDate->text();
        QString reconStmtEndDateReformated = reconStmtEndDate.sliced(4, 4)+reconStmtEndDate.sliced(0, 4);

        if(reconPaymentsStr == "1 NFCU Checking"){
            QString paymentsView =
            "WITH pmtViewQuery AS (SELECT id, bkRef, acctRef, txRef, txDate, SUBSTR(txDate, 5, 4) || SUBSTR(txDate, 1, 4) AS fmtTxDate, desc, memo, amount, reconciled FROM transactions "
            "WHERE txRef !=0 AND memo !=0 AND acctRef !=59 "
            "AND fmtTxDate <= :reconStmtEndDateReformated "
            "AND ((desc LIKE '%8113%' OR desc LIKE '%7005%' OR desc LIKE '%7826%') "
            "OR acctRef == 1 "
            "OR acctRef == 2 "
            "OR acctRef == 54 "
            "OR acctRef == 52 "
            "OR (acctRef == 53 AND desc NOT LIKE 'Costco%') "
            "OR acctRef == 69 "
            "OR acctRef == 22 "
            "OR acctRef == 23 "
            "OR acctRef == 24 "
            "OR acctRef == 25 "
            "OR acctRef == 26 "
            "OR acctRef == 27 "
            "OR acctRef == 28 "
            "OR acctRef == 29 "
            "OR acctRef == 30 "
            "OR acctRef == 31 "
            "OR acctRef == 32 "
            "OR acctRef == 33 "
            "OR acctRef == 34 "
            "OR acctRef == 35 "
            "OR acctRef == 36 "
            "OR acctRef == 38 "
            "OR acctRef == 39 "
            "OR acctRef == 41 "
            "OR acctRef == 42 "
            "OR acctRef == 43 "
            "OR acctRef == 44 "
            "OR acctRef == 46 "
            "OR acctRef == 47 "
            "OR acctRef == 48 "
            "OR acctRef == 49 "
            "OR acctRef == 50 "
            "OR acctRef == 56 "
            "OR acctRef == 57 "
            "OR acctRef == 60 "
            "OR acctRef == 68 "
            "OR acctRef == 73 "
            "OR acctRef == 75 "
            "OR acctRef == 76 "
            "OR acctRef == 77 "
            "OR acctRef == 79 "
            "OR acctRef == 80 "
            "OR acctRef == 82 "
            "OR acctRef == 83 "
            "OR acctRef == 86 "
            "OR acctRef == 87 "
            "OR acctRef == 88 "
            "OR acctRef == 90 "
            "OR acctRef == 93 "
            "OR acctRef == 94 "
            "OR acctRef == 96) "
            "AND reconciled IS NULL) \
            SELECT id, bkRef AS Bk, fmtTxDate, desc, amount AS Amt \
            FROM pmtViewQuery \
            WHERE bkRef LIKE :bkRef \
            Order By fmtTxDate ASC;";

            QString bankReconText = ui->lineEditReconAccount->text();
            QString bankReconTextSliced = bankReconText.sliced(0,2);
            int bankReconInt = bankReconTextSliced.toInt();

            QSqlQuery query(db);

            int sumOfPymtsUncleared = 0;

            query.prepare(paymentsView);
            query.bindValue(":bkRef", bankReconInt, QSql::InOut);
            query.bindValue(":reconStmtEndDateReformated", reconStmtEndDateReformated, QSql::InOut);

            if(!query.exec()){
                qDebug() << "Payments query didn't execute!";
                query.lastError();
            }
            else{
                query.next();
                QSqlQueryModel* model = new QSqlQueryModel();
                model->setQuery(std::move(query));
                ui->tableViewReconPayments->setModel(model);

                QAbstractItemModel* model2 = ui->tableViewReconPayments->model();

                for(int row = 0; row < model2->rowCount(); ++row){
                    QModelIndex index = model2->index(row, 4);
                    int value = model2->data(index).toInt();
                    if(value < 0){
                        value = -value;
                    }
                    sumOfPymtsUncleared += value;
                }
                ui->lineEditReconUnclearedPayments->setText(QString::number(sumOfPymtsUncleared));
                int tarBalInt = ui->lineEditReconTarBal->text().toInt();
                int curBalInt = ui->lineEditReconCurBal->text().toInt();
                ui->lineEditReconDiff->setText((QString::number(tarBalInt-curBalInt)));
                on_pushButtonReconRefreshDeposits_clicked();
            }
        }

        if(reconPaymentsStr == "4 HNB Checking"){
            QString paymentsView =
                "WITH pmtViewQuery AS (SELECT id, bkRef, acctRef, txRef, txDate, SUBSTR(txDate, 5, 4) || SUBSTR(txDate, 1, 4) AS fmtTxDate, desc, memo, amount, reconciled FROM transactions "
                "WHERE txRef !=0 AND memo !=0 AND acctRef !=59 "
                "AND fmtTxDate <= :reconStmtEndDateReformated "
                "AND ((desc LIKE '%0241%' OR desc LIKE '%9320%' OR desc LIKE '%7696%' OR desc LIKE '4073' OR desc LIKE '0393') "
                "OR acctRef == 51 "
                "OR acctRef == 53 "
                "OR acctRef == 72 "
                "OR acctRef == 76 "
                "OR acctRef == 6 "
                "OR acctRef == 7 "
                "OR acctRef == 8 "
                "OR acctRef == 9 "
                "OR acctRef == 10 "
                "OR acctRef == 11 "
                "OR acctRef == 12 "
                "OR acctRef == 13 "
                "OR acctRef == 14 "
                "OR acctRef == 15 "
                "OR acctRef == 16 "
                "OR acctRef == 17 "
                "OR acctRef == 18 "
                "OR acctRef == 74 "
                "OR acctRef == 77 "
                "OR acctRef == 92 "
                "OR acctRef == 95) "
                "AND reconciled IS NULL) \
                SELECT id, bkRef AS Bk, fmtTxDate, desc, amount AS Amt \
                FROM pmtViewQuery \
                WHERE bkRef LIKE :bkRef \
                Order By fmtTxDate ASC;";

            QString bankReconText = ui->lineEditReconAccount->text();
            QString bankReconTextSliced = bankReconText.sliced(0,2);
            int bankReconInt = bankReconTextSliced.toInt();

            QSqlQuery query(db);

            int sumOfPymtsUncleared = 0;

            query.prepare(paymentsView);
            query.bindValue(":bkRef", bankReconInt, QSql::InOut);
            query.bindValue(":reconStmtEndDateReformated", reconStmtEndDateReformated, QSql::InOut);

            if(!query.exec()){
                qDebug() << "Payments query didn't execute!";
                query.lastError();
            }
            else{
                query.next();
                QSqlQueryModel* model = new QSqlQueryModel();
                model->setQuery(std::move(query));
                ui->tableViewReconPayments->setModel(model);

                QAbstractItemModel* model2 = ui->tableViewReconPayments->model();

                for(int row = 0; row < model2->rowCount(); ++row){
                    QModelIndex index = model2->index(row, 4);
                    int value = model2->data(index).toInt();
                    if(value < 0){
                        value = -value;
                    }
                    sumOfPymtsUncleared += value;
                }
                ui->lineEditReconUnclearedPayments->setText(QString::number(sumOfPymtsUncleared));
                int tarBalInt = ui->lineEditReconTarBal->text().toInt();
                int curBalInt = ui->lineEditReconCurBal->text().toInt();
                ui->lineEditReconDiff->setText((QString::number(tarBalInt-curBalInt)));
                on_pushButtonReconRefreshDeposits_clicked();
            }
        }
    }
    db.close();
}


void MainWindow::on_pushButtonMarkPmtSel_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        int row;
        QString cleared = ui->lineEditReconPaymentsCleared->text();
        int clearedInt = cleared.toInt();

        row = ui->tableViewReconPayments->currentIndex().row();
        QVariant idInput = ui->tableViewReconPayments->model()->data(ui->tableViewReconPayments->model()->index(row, 0));
        QVariant markedPmtAmt = ui->tableViewReconPayments->model()->data(ui->tableViewReconPayments->model()->index(row, 4));
        int markedPmtAmtInt = markedPmtAmt.toInt();

        if(markedPmtAmtInt < 0){
            markedPmtAmtInt = -1 * markedPmtAmtInt;
            clearedInt += markedPmtAmtInt;
        }
        else{
            clearedInt += markedPmtAmtInt;
        }

        QSqlQuery query;
        query.prepare("UPDATE transactions SET reconciled = 1 WHERE id = :id");
        query.bindValue(":id", idInput, QSql::In);

        if(!query.exec()){
            qInfo() << "Query didn't execute";
        }
        else {
            ui->lineEditReconPaymentsCleared->setText(QString::number(clearedInt));
            on_pushButtonRefreshReconPayments_clicked();
            on_pushButtonRefreshReconPayments_clicked();
        }
    }
    db.close();
}

void MainWindow::on_pushButtonReconRefreshDeposits_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QVariant reconDeposits = ui->comboBoxReconAcct->currentText();
        QString reconDepositsStr = reconDeposits.toString();
        QString reconStmtEndDate = ui->lineEditReconStmtEndDate->text();
        QString reconStmtEndDateReformated = reconStmtEndDate.sliced(4, 4)+reconStmtEndDate.sliced(0, 4);

        if(reconDepositsStr == "1 NFCU Checking"){
            QString depositsView =
                "WITH depositsViewQuery AS "
                "(SELECT id, bkRef, acctRef, txRef, txDate, SUBSTR(txDate, 5, 4) || SUBSTR(txDate, 1, 4) AS fmtTxDate, desc, memo, amount, reconciled FROM transactions "
                "WHERE reconciled IS NULL AND "
                "fmtTxDate <= :reconStmtEndDateReformated AND "
                "(acctRef == 58 OR acctRef == 59 OR desc LIKE '%Dividend%' OR "
                "acctRef == 85 OR acctRef == 89 OR desc LIKE '%Interest%')) \
                SELECT id, bkRef AS Bk, txDate AS Date, desc, amount AS Amt \
                FROM depositsViewQuery \
                WHERE bkRef LIKE :bkRef \
                Order By txDate ASC;";

            QString bankReconText = ui->lineEditReconAccount->text();
            QString bankReconTextSliced = bankReconText.sliced(0,2);
            int bankReconInt = bankReconTextSliced.toInt();

            QSqlQuery query(db);

            int sumOfUnclearedDeposits = 0;

            query.prepare(depositsView);
            query.bindValue(":bkRef", bankReconInt, QSql::InOut);
            query.bindValue(":reconStmtEndDateReformated", reconStmtEndDateReformated, QSql::InOut);

            if(!query.exec()){
                qInfo() << "Deposits query didn't execute";
                qDebug() << query.lastError();
            }
            else {
                query.next();
                QSqlQueryModel* modelA = new QSqlQueryModel();
                modelA->setQuery(std::move(query));
                ui->tableViewReconDeposits->setModel(modelA);

                QAbstractItemModel* modelB = ui->tableViewReconDeposits->model();

                for(int row = 0; row < modelB->rowCount(); ++row){
                    QModelIndex index = modelB->index(row, 4);
                    int value = modelB->data(index).toInt();
                    if(value < 0){
                        value = -1 * value;
                    }
                    sumOfUnclearedDeposits += value;
                }
                QString begBalance = ui->lineEditReconBegBal->text();
                QString paymentsCleared = ui->lineEditReconPaymentsCleared->text();
                QString depositsCleared = ui->lineEditReconDepositsCleared->text();
                int begBalanceInt = begBalance.toInt();
                int paymentsClearedInt = paymentsCleared.toInt();
                int depositsClearedInt = depositsCleared.toInt();

                int currentBalanceInt = begBalanceInt - paymentsClearedInt + depositsClearedInt;
                QString currentBalanceStr = QString::number(currentBalanceInt);

                ui->lineEditReconUnclearedDeposits->setText(QString::number(sumOfUnclearedDeposits));
                ui->lineEditReconCurBal->setText(currentBalanceStr);
            }
        }

        if(reconDepositsStr == "4 HNB Checking"){
            QString depositsView =
                "WITH depositsViewQuery AS "
                "(SELECT id, bkRef, acctRef, txRef, txDate, SUBSTR(txDate, 5, 4) || SUBSTR(txDate, 1, 4) AS fmtTxDate, desc, memo, amount, reconciled FROM transactions "
                "WHERE reconciled IS NULL AND "
                "fmtTxDate <= :reconStmtEndDateReformated AND "
                "(acctRef == 61 OR acctRef == 62 OR desc LIKE '%HNB-INTEREST%' OR "
                "acctRef == 63 OR acctRef == 84)) \
                SELECT id, bkRef AS Bk, txDate AS Date, desc, amount AS Amt \
                FROM depositsViewQuery \
                WHERE bkRef LIKE :bkRef \
                Order By txDate ASC;";

            QString bankReconText = ui->lineEditReconAccount->text();
            QString bankReconTextSliced = bankReconText.sliced(0,2);
            int bankReconInt = bankReconTextSliced.toInt();

            QSqlQuery query(db);

            int sumOfUnclearedDeposits = 0;

            query.prepare(depositsView);
            query.bindValue(":bkRef", bankReconInt, QSql::InOut);
            query.bindValue(":reconStmtEndDateReformated", reconStmtEndDateReformated, QSql::InOut);

            if(!query.exec()){
                qInfo() << "Deposits query didn't execute";
                qDebug() << query.lastError();
            }
            else {
                query.next();
                QSqlQueryModel* modelA = new QSqlQueryModel();
                modelA->setQuery(std::move(query));
                ui->tableViewReconDeposits->setModel(modelA);

                QAbstractItemModel* modelB = ui->tableViewReconDeposits->model();

                for(int row = 0; row < modelB->rowCount(); ++row){
                    QModelIndex index = modelB->index(row, 4);
                    int value = modelB->data(index).toInt();
                    if(value < 0){
                        value = -1 * value;
                    }
                    sumOfUnclearedDeposits += value;
                }
                QString begBalance = ui->lineEditReconBegBal->text();
                QString paymentsCleared = ui->lineEditReconPaymentsCleared->text();
                QString depositsCleared = ui->lineEditReconDepositsCleared->text();
                int begBalanceInt = begBalance.toInt();
                int paymentsClearedInt = paymentsCleared.toInt();
                int depositsClearedInt = depositsCleared.toInt();

                int currentBalanceInt = begBalanceInt - paymentsClearedInt + depositsClearedInt;
                QString currentBalanceStr = QString::number(currentBalanceInt);

                ui->lineEditReconUnclearedDeposits->setText(QString::number(sumOfUnclearedDeposits));
                ui->lineEditReconCurBal->setText(currentBalanceStr);
            }
        }
    }
    db.close();
}


void MainWindow::on_pushButtonMarkDepSel_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        int row;
        QString cleared = ui->lineEditReconDepositsCleared->text();
        int clearedInt = cleared.toInt();

        row = ui->tableViewReconDeposits->currentIndex().row();
        QVariant idInput = ui->tableViewReconDeposits->model()->data(ui->tableViewReconDeposits->model()->index(row, 0));
        QVariant markedDepAmt = ui->tableViewReconDeposits->model()->data(ui->tableViewReconDeposits->model()->index(row, 4));
        int markedDepAmtInt = markedDepAmt.toInt();

        QSqlQuery query;
        query.prepare("UPDATE transactions SET reconciled = 1 WHERE id = :id");
        query.bindValue(":id", idInput, QSql::In);

        if(!query.exec()){
            qInfo() << "UPDATE Query didn't execute";
        }
        else {
            if(markedDepAmtInt < 0) {
                markedDepAmtInt = -1 * markedDepAmtInt;
                clearedInt += markedDepAmtInt;
            }
            else{
                clearedInt += markedDepAmtInt;
            }
            ui->lineEditReconDepositsCleared->setText(QString::number(clearedInt));
            on_pushButtonReconRefreshDeposits_clicked();
            on_pushButtonRefreshReconPayments_clicked();
        }
    }
    db.close();
}


void MainWindow::on_pushButtonReconPostpone_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Reconciliation Status Saved!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {

        QString pmtsCleared = ui->lineEditReconPaymentsCleared->text();
        QString pmtsUncleared = ui->lineEditReconUnclearedPayments->text();
        QString depsCleared = ui->lineEditReconDepositsCleared->text();
        QString depsUncleared = ui->lineEditReconUnclearedDeposits->text();
        QString begBalance = ui->lineEditReconBegBal_2->text();
        QString curBalance = ui->lineEditReconCurBal->text();
        QString tarBalance = ui->lineEditReconTarBal->text();
        QString difference = ui->lineEditReconDiff->text();

        QString saveReconStateQuery = "UPDATE Recon SET \
            PmtsCleared = :pmtsCleared, \
            PmtsUncleared = :pmtsUncleared, \
            DepsCleared = :depsCleared, \
            DepsUncleared = :depsUncleared, \
            BegBalance = :begBalance, \
            CurBalance = :curBalance, \
            TarBalance = :tarBalance, \
            Difference = :difference \
            WHERE id = 1;";

        QSqlQuery updateQuery(db);
        updateQuery.prepare(saveReconStateQuery);

        updateQuery.bindValue(":pmtsCleared", pmtsCleared);
        updateQuery.bindValue(":pmtsUncleared", pmtsUncleared);
        updateQuery.bindValue(":depsCleared", depsCleared);
        updateQuery.bindValue(":depsUncleared", depsUncleared);
        updateQuery.bindValue(":begBalance", begBalance);
        updateQuery.bindValue(":curBalance", curBalance);
        updateQuery.bindValue(":tarBalance", tarBalance);
        updateQuery.bindValue(":difference", difference);


        if(!updateQuery.exec()) {
            qWarning() << "Update Query didn't execute";
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
    }
    db.close();
}



void MainWindow::on_pushButtonReconFinished_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Reconciliation Status Reset to NULL!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else{
        QString resetReconStateQuery =
            "UPDATE Recon SET \
            PmtsCleared = :pmtsCleared, \
            PmtsUncleared = :pmtsUncleared, \
            DepsCleared = :depsCleared, \
            DepsUncleared = :depsUncleared, \
            BegBalance = :begBalance, \
            CurBalance = :curBalance, \
            TarBalance = :tarBalance, \
            Difference = :difference \
            WHERE id =1;";

        QSqlQuery query(db);
        query.prepare(resetReconStateQuery);
        if(!query.exec()){
                qInfo() << "Update Query didn't execute";
        }
        else{
            query.bindValue(":pmtsCleared", '0', QSql::InOut);
            query.bindValue(":pmtsUnCleared", '0', QSql::InOut);
            query.bindValue(":depsCleared", '0', QSql::InOut);
            query.bindValue(":depsUncleared", '0', QSql::InOut);
            query.bindValue(":begBalance", '0', QSql::InOut);
            query.bindValue(":curBalance", '0', QSql::InOut);
            query.bindValue(":tarBalance", '0', QSql::InOut);
            query.bindValue(":difference", '0', QSql::InOut);

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



void MainWindow::on_pushButtonReconResume_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Reconciliation Status Restored!");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/Desktop/money.sqlite3");
    db.open();
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
    }
    else {
        QString restoreReconStateQuery = "SELECT * FROM Recon WHERE id = :id;";

        QSqlQuery restoreQuery(db);
        restoreQuery.prepare(restoreReconStateQuery);
        restoreQuery.bindValue(":id", 1);

        if(!restoreQuery.exec()) {
            qWarning() << "Update Query didn't execute";
            return;
        }

        if(!restoreQuery.next()) {
            qWarning() << "Record not found";
            return;
        }
        else {
            QString pmtsCleared = restoreQuery.value(1).toString();
            QString pmtsUncleared = restoreQuery.value(2).toString();
            QString depsCleared = restoreQuery.value(3).toString();
            QString depsUncleared = restoreQuery.value(4).toString();
            QString begBalance = restoreQuery.value(5).toString();
            QString curBalance = restoreQuery.value(6).toString();
            QString tarBalance = restoreQuery.value(7).toString();
            QString difference = restoreQuery.value(8).toString();

            ui->lineEditReconPaymentsCleared->setText(pmtsCleared);
            ui->lineEditReconUnclearedPayments->setText(pmtsUncleared);
            ui->lineEditReconDepositsCleared->setText(depsCleared);
            ui->lineEditReconUnclearedDeposits->setText(depsUncleared);
            ui->lineEditReconBegBal_2->setText(begBalance);
            ui->lineEditReconCurBal->setText(curBalance);
            ui->lineEditReconTarBal->setText(tarBalance);
            ui->lineEditReconDiff->setText(difference);

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

