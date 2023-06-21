#ifndef MONEY_BANK_H
#define MONEY_BANK_H

#include <QDialog>
#include "mydb.h"
#include <iostream>
using namespace std;

namespace Ui {
class Bank;
}

class Bank : public QDialog
{
    Q_OBJECT

public:
    explicit Bank(QWidget *parent = nullptr);
    ~Bank();

    void deposit(int amount);
    [[nodiscard]] int getBalance() const;
    void withdrawal(int amount);
    void setBalance(int amount);
    [[nodiscard]] string getName() const;
    

private slots:
    

private:
    Ui::Bank *ui;
    int m_bkRef{};
    string m_name;
    string m_created;
    int m_balance{};
    bool m_active{false};
};


#endif //MONEY_BANK_H
