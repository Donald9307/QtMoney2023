/********************************************************************************
** Form generated from reading UI file 'dialogBanks.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGBANKS_H
#define UI_DIALOGBANKS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_dialogBank
{
public:
    QLabel *labelBankRef;
    QLineEdit *lineEditBankRef;
    QLabel *labelName;
    QLineEdit *lineEditName;
    QLabel *labelCreated;
    QLineEdit *lineEditCreated;
    QLabel *labelBalance;
    QLineEdit *lineEditBalance;
    QLabel *labelActive;
    QLineEdit *lineEditActive;
    QPushButton *pushButton;

    void setupUi(QDialog *dialogBank)
    {
        if (dialogBank->objectName().isEmpty())
            dialogBank->setObjectName("dialogBank");
        dialogBank->resize(400, 300);
        labelBankRef = new QLabel(dialogBank);
        labelBankRef->setObjectName("labelBankRef");
        labelBankRef->setGeometry(QRect(20, 30, 76, 19));
        lineEditBankRef = new QLineEdit(dialogBank);
        lineEditBankRef->setObjectName("lineEditBankRef");
        lineEditBankRef->setGeometry(QRect(140, 30, 113, 27));
        labelName = new QLabel(dialogBank);
        labelName->setObjectName("labelName");
        labelName->setGeometry(QRect(20, 70, 76, 19));
        lineEditName = new QLineEdit(dialogBank);
        lineEditName->setObjectName("lineEditName");
        lineEditName->setGeometry(QRect(140, 70, 231, 27));
        labelCreated = new QLabel(dialogBank);
        labelCreated->setObjectName("labelCreated");
        labelCreated->setGeometry(QRect(20, 110, 76, 19));
        lineEditCreated = new QLineEdit(dialogBank);
        lineEditCreated->setObjectName("lineEditCreated");
        lineEditCreated->setGeometry(QRect(140, 110, 113, 27));
        labelBalance = new QLabel(dialogBank);
        labelBalance->setObjectName("labelBalance");
        labelBalance->setGeometry(QRect(20, 150, 76, 19));
        lineEditBalance = new QLineEdit(dialogBank);
        lineEditBalance->setObjectName("lineEditBalance");
        lineEditBalance->setGeometry(QRect(140, 150, 113, 27));
        labelActive = new QLabel(dialogBank);
        labelActive->setObjectName("labelActive");
        labelActive->setGeometry(QRect(20, 190, 76, 19));
        lineEditActive = new QLineEdit(dialogBank);
        lineEditActive->setObjectName("lineEditActive");
        lineEditActive->setGeometry(QRect(140, 190, 113, 27));
        pushButton = new QPushButton(dialogBank);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(20, 250, 351, 27));

        retranslateUi(dialogBank);

        QMetaObject::connectSlotsByName(dialogBank);
    } // setupUi

    void retranslateUi(QDialog *dialogBank)
    {
        dialogBank->setWindowTitle(QCoreApplication::translate("dialogBank", "Dialog", nullptr));
        labelBankRef->setText(QCoreApplication::translate("dialogBank", "Bank Ref", nullptr));
        labelName->setText(QCoreApplication::translate("dialogBank", "Name", nullptr));
        labelCreated->setText(QCoreApplication::translate("dialogBank", "Created", nullptr));
        labelBalance->setText(QCoreApplication::translate("dialogBank", "Balance", nullptr));
        labelActive->setText(QCoreApplication::translate("dialogBank", "Active", nullptr));
        pushButton->setText(QCoreApplication::translate("dialogBank", "Save To Database", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dialogBank: public Ui_dialogBank {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGBANKS_H
