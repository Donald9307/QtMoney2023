#ifndef MONEYCPP_TRANSACTION_H
#define MONEYCPP_TRANSACTION_H
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;


class Transaction {
public:
    // Constructors
    Transaction(int bkRef, int amount) {
        m_bkRef = bkRef;
        m_amount = amount;
    };
    Transaction(int bkRef, int acctRef, int txRef, string date, string desc, string memo, int amount)
        : m_bkRef(bkRef), m_acctRef(acctRef), m_txRef(txRef), m_date(date), m_desc(desc), m_memo(memo), m_amount(amount) {}

    bool operator<(const Transaction& other) const {
        return m_txRef < other.m_txRef;
    }

    // Member attributes
    int m_bkRef{};
    int m_acctRef{};
    int m_txRef{};
    string m_date;
    string m_desc;
    string m_memo;
    int m_amount{};

    // Methods
};



#endif //MONEYCPP_TRANSACTION_H
