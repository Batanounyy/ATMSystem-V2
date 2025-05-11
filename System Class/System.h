//
// Created by Gaming PC on 5/11/2025.
//

#ifndef SYSTEM_H
#define SYSTEM_H
#include <iostream>
using namespace std;


class System {
private:
    string SessionNumber;
    string MainFileArr[3];
    string MainSystemKey;
    string NumberOfAccounts;
    string AdminUsername= "admin";
    string AdminPassword= "admin";

    bool running= 1;
    bool loggedIn= 0;
    int ListNum=0;
    string SendArr[200];


    void readMainFile();
    void AdminMenu();
    void AdminAddAccount();
    void AdminRemoveAccount();
    void AdminShowAllAccounts();
    void AdminShowLogs();
    void AdminChangeKey();
    string generate_random_key();

    void MainMenu();
    void LoginMenu();
    void HomeMenu();
    void AboutMenu();

    void CheckBalance();
    void Deposit();
    void Withdraw();
    void ChangePIN();
    void Transactions();
    void Logout();

    void LOG(string Text);
public:
    void run();

};



#endif //SYSTEM_H
