#ifndef MONEYCPP_ACCOUNT_H
#define MONEYCPP_ACCOUNT_H
#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

class Account {
public:
    // Constructor
    Account() = default;
    Account(int bkRef, int acctRef, string& name, int balance){
        m_bkRef = bkRef;
        m_acctRef = acctRef;
        m_name = name;
        m_balance = balance;
    }

    //Member functions
    void addTxAmount(int txAmount);
    void subtractTxAmount(int txAmount);

    // Comparator for Account objects
    bool operator<(const Account& other) const {
        return m_acctRef < other.m_acctRef;
    }

    // Member attributes
    int m_bkRef{};
    int m_acctRef{};
    string m_name;
    int m_balance{};

};



#endif //MONEYCPP_ACCOUNT_H
