#ifndef DIALOGBANKS_H
#define DIALOGBANKS_H

#include "qpushbutton.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    connect(Dialog, &QPushButton::clicked, this, [this]) {
        //Instantiate the QDialog object
        dialog = new QDialog(this);
        //Show the QDialog
        dialog->show();
    }

private:
    Ui::Dialog *ui;
};

#endif // DIALOGBANKS_H
