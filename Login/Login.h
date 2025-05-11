#ifndef LOGIN_H
#define LOGIN_H
#include <fstream>
using namespace std;


class Login {
private:
    const int MAXIMUMACC = 100;
    string Data1Arr[100];
    string Data2Arr[100];

    int ListNum=0;
    bool loginstatus=false;

public:
    void login(string accnum, string pin );

    bool VerifyCredentials(string accnum, string pin);
    void ReadFiles();

    bool getLoginStatus();
    int getListNum();
};



#endif //LOGIN_H
