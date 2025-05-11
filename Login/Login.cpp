#include <iostream>
#include <fstream>
#include <string>
#include "Login.h"

using namespace std;

void Login::login(string accnum, string pin ) {
    if (accnum=="" || pin=="") {
        cout << "Please Enter Account Number and PIN" << endl;
    }
    else if (accnum=="admin" && pin=="admin") {
        cout << "Admin Login" << endl;
        loginstatus = true;
        ListNum=0;
    }
    else {
        ReadFiles();

        if (VerifyCredentials(accnum, pin)==1) {
            cout << "Login successful!" << endl;
            loginstatus = true;
        }
        else if (VerifyCredentials(accnum, pin)==0) {
            cout << "Login failed!" << endl;
            loginstatus = false;
        }
        else {
            cout << "Error occurred!" << endl;
            loginstatus = false;
        }
    }
}

bool Login::VerifyCredentials(string accnum, string pin) {

    for (int i=0; i<MAXIMUMACC; i++) {
        if (Data1Arr[i]==accnum && Data2Arr[i]==pin) {
            ListNum = i;
            return 1;
        }
    }
    return 0;
}
void Login::ReadFiles() {
    ifstream Data1("../Login/LoginData/LD1.data");
    ifstream Data2("../Login/LoginData/LD2.data");
    if (!Data1.is_open()||!Data2.is_open()) {
        cerr << "Error opening files" << endl;
    }
    else {
        for (int i = 0; i < MAXIMUMACC; i++) {
            Data1 >> Data1Arr[i];
            if (Data1.eof()) {
                break;
            }
        }
        for (int i = 0; i < MAXIMUMACC; i++) {
            Data2 >> Data2Arr[i];
            if (Data2.eof()) {
                break;
            }
        }
    }
}

bool Login::getLoginStatus() {
    return loginstatus;
}
int Login::getListNum() {
    return ListNum;
}

