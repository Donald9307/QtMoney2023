# QtMoney2023
Financial Management Application Written in Qt6.5 C++

To run QtMoney2023 you must meet the following prerequisites:
  1. Be running a Linux distro (preferably Debian 11 or higher OS).
  2. Have installed Qt Creator version 9.0.2 or higher. You must register a Qt account at https://login.qt.io/register to obtain the opensource version of Qt Creator. 
  3. Have installed Qt6. The free version link is https://www.qt.io-download-open-source, available from your Qt account.  
  4. Have installed Sqlite3 database server.  From a terminal session, issue $sudo apt install sqlite3
  5. Have cmake version 3.5 or higher installed at /usr/bin. From a terminal session, issue $sudo apt install cmake
  6. Move money.sqlite3 database to your ~/Desktop folder

Once prerequisites are met, build QtMoney2023.

Notes On Naming Accounts:

  1. When creating and naming accounts, it is best to use the format <Asset/Liability>-<account title>.  For example, BankOne-Dining; BankOne-Groceries, etc.  The reason for this is so you can differentiate between accounts in different banks that may have the same account title.
  2. When creating and naming Credit Card accounts, use the naming paradigm as for regular accounts but add -<last four digits of the card>.  For example, BankOne-VISA-9876; BankOne-VISA-1234. This will differentiate between credit cards in the event you own more than one VISA card from the same bank. 
  3. When memorizing transactions for later use, you may want to use a short name in all CAPS in the Description field of either the expense transaction or credit transaction. Using a short name will help you to remember the transaction. You can always View Memorized Transactions if you forget the name. 
