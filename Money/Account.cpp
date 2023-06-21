#include <iomanip>
#include "Account.h"
using namespace std;


void Account::addTxAmount(int txAmount) {
    m_balance += txAmount;
}

void Account::subtractTxAmount(int txAmount) {
    m_balance -= txAmount;
}
