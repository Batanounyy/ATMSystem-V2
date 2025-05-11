//
// Created by Gaming PC on 5/11/2025.
//

#ifndef SYSTEM_H
#define SYSTEM_H
#include <iostream>
using namespace std;


class System {
    private:
    bool running= 1;
    bool loggedIn= 0;
    int ListNum=0;
    string SendArr[200];
    string MainSystemKey="Batanouny";

    public:
    void run();

    void AdminMenu();

    void MainMenu();
    void LoginMenu();
    void HomeMenu();

    void CheckBalance();
    void Deposit();
    void Withdraw();
    void ChangePIN();
    void Transactions();
    void Logout();

};



#endif //SYSTEM_H
