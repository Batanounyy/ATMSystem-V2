#ifndef ACCOUNT_H
#define ACCOUNT_H
using namespace std;
#include <iostream>


class Account {
private:
    int ListNum;
    string filelocation;
    string UserArr[105];

    string KEY, ACCNUM, PIN, BALANCE , NAME;
    string Transactions[100];
public:
    void constructor();
    void startAccount(int listnum);
    void endAccount();


    string getkey();
    string getAccNum();
    string getPIN();
    string getBalance();
    string getName();

    void setPIN(string newPIN);
    void setBalance(string newBalance);

    string displayTransactions(int num);
    void addTransaction(string Trancation);

};



#endif //ACCOUNT_H
