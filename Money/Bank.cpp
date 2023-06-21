#include "Bank.h"
#include <iomanip>
#include <iostream>

int Bank::getBalance() const {
    return m_balance;
}

void Bank::deposit(int amount) {
    m_balance += amount;
}

void Bank::setBalance(int amount) {
    m_balance = amount;
}

void Bank::withdrawal(int amount) {
    m_balance -= amount;
}

string Bank::getName() const {
    return m_name;
}
