#include "Account.h"
#include <fstream>
#include <iostream>



void Account::startAccount(int listnum) {
    ListNum = listnum;
    filelocation = "../Data/U"+to_string(ListNum)+".data";
    ifstream UserFile(filelocation);
    int lineCount = 0;
    if (UserFile.is_open()) {
        string line;
        while (getline(UserFile, line) && lineCount < 200) {
            UserArr[lineCount] = line;
            lineCount++;
        }
        UserFile.close();
    }
    else {
        cerr << "Unable to open file" << std::endl;
    }

    KEY=UserArr[0];
    ACCNUM=UserArr[1];
    NAME=UserArr[2];
    BALANCE=UserArr[3];
    for (int i=4; i<95; i++) {
        Transactions[i-4]=UserArr[i];
    }

}
void Account::endAccount() {
    ofstream UserFile(filelocation);

    UserArr[0]=KEY;
    UserArr[1]=ACCNUM;
    UserArr[2]=NAME;
    UserArr[3]=BALANCE;
    for (int i=4; i<95; i++) {
        UserArr[i]=Transactions[i-4];
    }
    if (UserFile.is_open()) {
        for (int i = 0; i < 105; i++) {
            UserFile << UserArr[i] << endl;
        }
        cout << "Account data saved successfully." << endl;
        UserFile.close();
    }
    else {
        cerr << "Unable to open file" << std::endl;
    }
}

void Account::constructor() {
    ListNum=0;
}

string Account::getkey() {
    return KEY;
}
string Account::getAccNum() {
    return ACCNUM;
}
string Account::getPIN(){
    return PIN;
}
string Account::getBalance() {
    return BALANCE;
}
string Account::getName() {
    return NAME;
}

void Account::setPIN(string newPIN) {
    PIN = newPIN;
}
void Account::setBalance(string newBalance) {
    BALANCE = newBalance;
}

string Account::displayTransactions(int num) {
    return Transactions[num];
}
void Account::addTransaction(string Trancation) {

    for (int i = 94; i > 0; i--) {
        Transactions[i] = Transactions[i - 1];
    }

    Transactions[0] = Trancation;

}
