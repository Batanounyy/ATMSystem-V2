#include "System.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../Login/Login.h"
#include "../Encryption/encrypt.h"
#include "../Account Class/Account.h"
#include <ctime>

Account account;

void System::LoginMenu() {
    cout << "Please Enter Account Number: ";
    string AccNum;
    cin >> AccNum;
    AccNum = encrypt(AccNum, MainSystemKey);
    cout << "Please Enter PIN: ";
    string AccPIN;
    cin >> AccPIN;
    AccPIN = encrypt(AccPIN, MainSystemKey);
    Login login;
    login.login(AccNum, AccPIN);
    if (login.getLoginStatus()==1) {
        loggedIn= login.getLoginStatus();
        ListNum = login.getListNum();
    }
}
void System::MainMenu() {
    cout << "Welcome to the Main Menu!" << endl;
    cout << "1. Login" << endl;
    cout << "2. About" << endl;
    cout << "3. Exit" << endl;
    int choice;
    cin >> choice;
    switch (choice) {
        case 1:
            LoginMenu();
            break;
        case 2:
            cout << "Hi" << endl;
            break;
        case 3:
            running = false;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}
void System::CheckBalance() {
    string EncBalance =account.getBalance();
    string Key = account.getkey();
    cout << "Current Balance = "<< decrypt(EncBalance, Key) << "EGP" <<endl;
}
void System::Deposit() {
    string EncBalance =account.getBalance();
    string Key = account.getkey();
    cout << "Current Balance = "<< decrypt(EncBalance, Key) << "EGP" <<endl;
    cout << "Enter Value to Deposite"<<endl;
    float Deposit;
    string Balance=decrypt(EncBalance, Key);
    float BalanceFloat = stof(Balance);
    cin >> Deposit;
    float final = BalanceFloat + Deposit;
    Balance=to_string(final);
    EncBalance = encrypt(Balance, Key);
    account.setBalance(EncBalance);
    time_t currentTime = time(0);
    char* dt = ctime(&currentTime);
    string Transaction = "Deposit: " + to_string(Deposit) + " : " +dt;
    Transaction = encrypt(Transaction, Key);
    account.addTransaction(Transaction);
}
void System::Withdraw() {
    string EncBalance =account.getBalance();
    string Key = account.getkey();
    cout << "Current Balance = "<< decrypt(EncBalance, Key) << "EGP" <<endl;
    cout << "Enter Value to Withdraw"<<endl;
    float Withdraw;
    string Balance=decrypt(EncBalance, Key);
    float BalanceFloat = stof(Balance);
    cin >> Withdraw;
    float final = BalanceFloat - Withdraw;
    Balance=to_string(final);
    EncBalance = encrypt(Balance, Key);
    account.setBalance(EncBalance);
    time_t currentTime = time(0);
    char* dt = ctime(&currentTime);
    string Transaction = "Withdraw: " + to_string(Withdraw) + " : " +dt;
    Transaction = encrypt(Transaction, Key);
    account.addTransaction(Transaction);

}
void System::ChangePIN() {
    ifstream file("../Login/LoginData/LD2.data");
    string PASSARR[100];
    for (int i = 0; i < 100; i++) {
        file >> PASSARR[i];
        if (file.eof()) {
            break;
        }
    }
    string AccRealPIN = PASSARR[ListNum];
    cout << "To Change PIN, Please enter the old PIN: ";
    string AccPIN;
    cin >> AccPIN;
    AccPIN = encrypt(AccPIN, MainSystemKey);
    if (AccPIN==AccRealPIN) {
        string PIN1, PIN2;
        cout << "Enter New PIN: ";
        cin >> PIN1;
        cout << "Re-enter New PIN: ";
        cin >> PIN2;
        if (PIN1==PIN2) {
            PIN1 = encrypt(PIN1, MainSystemKey);
            ofstream file("../Login/LoginData/LD2.data");
            for (int i = 0; i < 100; i++) {
                if (i==ListNum) {
                    file << PIN1 << endl;
                }
                else {
                    file << PASSARR[i] << endl;
                }
            }
            cout << "PIN changed successfully!" << endl;
        }
        else {
            cout << "New PINs do not match!" << endl;
        }
    }
    else {
        cout << "Enterd PIN is incorrect" << endl;
    }

}
void System::Transactions() {
    cout << "Last Transactions: " << endl;
    string Key = account.getkey();
    for (int i = 0; i < 95; i++) {
        string Transaction = account.displayTransactions(i);
        if (Transaction != "") {
            Transaction = decrypt(Transaction, Key);
            cout << i+1 << "] "<<Transaction << endl;
        }
    }
}
void System::Logout() {
    account.endAccount();
    cout << "Logging Out" << endl;
    loggedIn=0;
}
void System::HomeMenu() {
    string EncName =account.getName();
    string Key = account.getkey();
    cout << "Hello " << decrypt(EncName, Key) <<", Please Choose an operation" << endl;
    cout << "1. Check Balance" << endl;
    cout << "2. Deposit" << endl;
    cout << "3. Withdraw" << endl;
    cout << "4. Change PIN" << endl;
    cout << "5. Last Transactions (Max 50)" << endl;
    cout << "6. Logout" << endl;
    int choice;
    cin >> choice;
    switch (choice) {
        case 1: {CheckBalance(); HomeMenu();break;}
        case 2: {Deposit();HomeMenu();break;}
        case 3: {Withdraw();HomeMenu();break;}
        case 4: {ChangePIN();HomeMenu();break;}
        case 5: {Transactions();HomeMenu();break;}
        case 6: {Logout();break;}
    }
}

void System::run() {

    while (running) {
        MainMenu();
        while (loggedIn==1) {
            if (ListNum==0) {
                AdminMenu();
            }
            account.constructor();
            account.startAccount(ListNum);
            HomeMenu();
        }
    }
    cout << "System shutting down." << endl;
}

void System::AdminMenu() {
    cout << "Admin Menu only adds New Accounts or remove older ones" << endl;
    cout << "1. Add New Account" << endl;
    cout << "2. Remove Account" << endl;
    cout << "3. Show All Accounts" << endl;
    cout << "4. Show All Transactions" << endl;
    cout << "5. Show LOGS" << endl;
    cout << "6. Exit" << endl;
    int choice;
    cin >> choice;
    switch (choice) {
        case 1: {
            cout << "Adding New Account" << endl;
            string AccNum, PIN, Name, Balance;
            cout << "Enter Account Number: ";
            cin >> AccNum;
            cout << "Enter PIN: ";
            cin >> PIN;
            cout << "Enter Name: ";
            cin >> Name;
            cout << "Enter Balance: ";
            cin >> Balance;
            ofstream file("../Login/LoginData/LD1.data", ios::app);
            file << encrypt(AccNum, MainSystemKey) << endl;
            file.close();
            file.open("../Login/LoginData/LD2.data", ios::app);
            file << encrypt(PIN, MainSystemKey) << endl;
            file.close();
        }
        break;
        case 2:
        {
            cout << "Removing Account" << endl;
        }
        break;
        case 3:
        {
            cout << "Showing All Accounts" << endl;
        }
        break;
        case 4:
        {
            cout << "Showing All Transactions" << endl;
        }
        break;
        case 5:
        {
            cout << "Showing LOGS" << endl;
        }
        break;
    }
}