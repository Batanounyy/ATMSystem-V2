#include "System.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../Login/Login.h"
#include "../Encryption/encrypt.h"
#include "../Account Class/Account.h"
#include <ctime>
#include <random>
#include <algorithm>

Account account;
void System::readMainFile()
{

    ifstream MainFile("../Data/Don't Open this!");
    LOG("Main File Opening");
    if (!MainFile.is_open()) {
        LOG("Main File Didnt Open due to an error");
        cerr << "Error opening files" << endl;
    }
    else
    {
        LOG("Main File Opened");
        for (int i = 0; i < 3; i++) {
            MainFile >> MainFileArr[i];
            if (MainFile.eof()) {
                LOG("Main File Moved to Array");
                break;
            }
        }
        MainSystemKey = MainFileArr[0];
        LOG("Main Key Set");
        NumberOfAccounts = MainFileArr[1];
        LOG("Number of Accounts Set");
        SessionNumber = MainFileArr[2];
        LOG("Number of Session Set");

        string tempp = MainFileArr[2];
        int temp = stoi(tempp);
        temp++;
        SessionNumber = to_string(temp);
    }
} //LOG DONE

void System::run() {

    while (running) {
        ListNum=-1;
        LOG("Running System Started");
        readMainFile();
        MainMenu();
        while (loggedIn==1) {
            if (ListNum==0) {
                LOG("Admin Login");
                AdminMenu();
            }
            if (ListNum==-1) {
                LOG("Failed Login");
                loggedIn=0;
                break;
            }
            else
            {
                LOG("Login Success");
                account.constructor();
                account.startAccount(ListNum);
                LOG("Account Set");
                HomeMenu();
            }
        }
    }
    //Save Session Number in DONT OPEN THIS!
    ofstream MainFile("../Data/Don't Open this!");
    if (MainFile.is_open()) {
        MainFile << MainSystemKey << endl;
        MainFile << NumberOfAccounts << endl;
        MainFile << SessionNumber << endl;
        cout << "Main file updated successfully." << endl;
        MainFile.close();
        LOG("Main File Updated");
    }
    else {
        cerr << "Unable to open main file" << endl;
        LOG("Failed to update Main file");
    }
    cout << "System shutting down." << endl;
    LOG("System shutting down");
} //LOG DONE
void System::MainMenu() {
    loggedIn=false;
    LOG(" Main Menu Started");
    cout << "Welcome to the Main Menu!" << endl;
    cout << "1. Login" << endl;
    cout << "2. About" << endl;
    cout << "3. Exit" << endl;
    int choice;
    cin >> choice;
    LOG(" User Made a choice");
    switch (choice) {
    case 1:
        LoginMenu();
        LOG(" User Chose Login Menu");
        break;
    case 2:
        AboutMenu();
        LOG(" User Chose About Menu");
        break;
    case 3:
        LOG(" User Chose to Exit");
        running = false;
        break;
    default:
        cout << "Invalid choice. Please try again." << endl;
        LOG(" User gave invalid choice");
    }
}//LOG DONE
void System::LoginMenu() {
    cout << "Please Enter Account Number: ";
    string Num;
    cin >> Num;
    LOG(" User Entered Account Number");
    string AccNum = encrypt(Num, MainSystemKey);
    cout << "Please Enter PIN: ";
    string PIN;
    cin >> PIN;
    LOG(" User Entered PIN");
    string AccPIN = encrypt(PIN, MainSystemKey);
    Login login;
    LOG("Login Object Created");
    if(PIN==AdminPassword && Num==AdminUsername) {
        login.login("admin", "admin");
        LOG(" Admin Login");
    }
    else{
    login.login(AccNum, AccPIN);
        LOG(" User Login");
}
    if (login.getLoginStatus()==1) {
        loggedIn= login.getLoginStatus();
        ListNum = login.getListNum();
        LOG(" User Login Success");
    }
}//LOG DONE
void System::AboutMenu()
{
    //Make some description about the whole project and all codes here in cout
    cout << "This is a simple ATM system that allows users to perform basic banking operations." << endl;
    cout << "It includes features such as checking balance, depositing, withdrawing, and changing PIN." << endl;
    cout << "The system is designed to be user-friendly and secure." << endl;
    cout << "Developed by:" << endl;
    cout << "Abdelrahman ElBatanouny" << endl;
    cout << "Rodaina Aly" << endl;
    cout << "Nour Sayed" << endl;
    cout << "Short Bilal ElSheikh" << endl;

}

void System::HomeMenu() {
    string EncName =account.getName();
    LOG(" Home Menu Started");
    string Key = account.getkey();
    LOG(" Key Set");
    cout << "Hello " << decrypt(EncName, Key) <<", Please Choose an operation" << endl;
    cout << "1. Check Balance" << endl;
    cout << "2. Deposit" << endl;
    cout << "3. Withdraw" << endl;
    cout << "4. Change PIN" << endl;
    cout << "5. Last Transactions (Max 50)" << endl;
    cout << "6. Logout" << endl;
    LOG(" User Chose Home Menu");
    int choice;
    cin >> choice;
    LOG(" User Made a choice");
    switch (choice) {
    case 1: {CheckBalance(); HomeMenu(); LOG(" User Chose Check Balance");break;}
    case 2: {Deposit();HomeMenu(); LOG(" User Chose Deposit");break;}
    case 3: {Withdraw();HomeMenu(); LOG(" User Chose Withdraw");break;}
    case 4: {ChangePIN();HomeMenu(); LOG(" User Chose Change PIN");break;}
    case 5: {Transactions();HomeMenu(); LOG(" User Chose Show Transactions");break;}
    case 6: {Logout(); LOG(" User Chose Logout");break;}
    }
}//LOG DONE
void System::CheckBalance() {
    string EncBalance =account.getBalance();
    LOG(" Check Balance Started");
    string Key = account.getkey();
    LOG(" Key Set");
    cout << "Current Balance = "<< decrypt(EncBalance, Key) << "EGP" <<endl;
    LOG(" User Checked Balance");
}//LOG DONE
void System::Deposit() {
    string EncBalance =account.getBalance();
    LOG(" Deposit Started");
    string Key = account.getkey();
    LOG(" Key Set");
    cout << "Current Balance = "<< decrypt(EncBalance, Key) << "EGP" <<endl;
    cout << "Enter Value to Deposite (Enter 0 to cancel)"<<endl;
    float Deposit;
    cin >> Deposit;
    LOG(" User Entered Deposit Value");
    if (Deposit == 0)
    {
        LOG(" User Canceled Deposit");
        return;
    }
    string Balance=decrypt(EncBalance, Key);
    float BalanceFloat = stof(Balance);
    float final = BalanceFloat + Deposit;
    LOG(" Final Value Calculated");
    Balance=to_string(final);
    EncBalance = encrypt(Balance, Key);
    LOG(" Balance Encrypted");
    account.setBalance(EncBalance);
    LOG(" Balance Set");
    time_t currentTime = time(0);
    char* dt = ctime(&currentTime);
    string Transaction = "Deposit: " + to_string(Deposit) + " : " +dt;
    Transaction = encrypt(Transaction, Key);
    account.addTransaction(Transaction);
    LOG(" Transaction Added");
}//LOG DONE
void System::Withdraw() {
    string EncBalance =account.getBalance();
    string Key = account.getkey();
    LOG(" Withdraw Started");
    cout << "Current Balance = "<< decrypt(EncBalance, Key) << "EGP" <<endl;
    cout << "Enter Value to Withdraw (Enter 0 to cancel)"<<endl;
    float Withdraw;
    cin >> Withdraw;
    LOG(" User Entered Withdraw Value");
    if (Withdraw == 0)
    {
        LOG(" User Canceled Withdraw");
        return;
    }
    else if (Withdraw > stof(decrypt(EncBalance, Key))) {
        cout << "Insufficient funds!" << endl;
        LOG(" User Entered Insufficient Funds");
        return;
    }
    {

    }
    string Balance=decrypt(EncBalance, Key);
    float BalanceFloat = stof(Balance);
    float final = BalanceFloat - Withdraw;
    LOG(" Final Value Calculated");
    Balance=to_string(final);
    EncBalance = encrypt(Balance, Key);
    LOG(" Balance Encrypted");
    account.setBalance(EncBalance);
    LOG(" Balance Set");
    time_t currentTime = time(0);
    char* dt = ctime(&currentTime);
    string Transaction = "Withdraw: " + to_string(Withdraw) + " : " +dt;
    Transaction = encrypt(Transaction, Key);
    account.addTransaction(Transaction);
    LOG(" Transaction Added");

}//LOG DONE
void System::ChangePIN() {
    ifstream file("../Login/LoginData/LD2.data");
    LOG(" Change PIN Started");
    string PASSARR[100];
    LOG(" PIN File Opened");
    for (int i = 0; i < 100; i++) {
        file >> PASSARR[i];
        if (file.eof()) {
            break;
        }
    }
    LOG(" PIN File Moved to Array");
    string AccRealPIN = PASSARR[ListNum];
    cout << "To Change PIN, Please enter the old PIN: (Enter 0 to cancel)";
    string AccPIN;
    cin >> AccPIN;
    LOG(" User Entered Old PIN");
    if (AccPIN == "0")
    {
        LOG(" User Canceled Change PIN");
        return;
    }
    AccPIN = encrypt(AccPIN, MainSystemKey);
    if (AccPIN==AccRealPIN) {
        string PIN1, PIN2;
        cout << "Enter New PIN: ";
        LOG(" User Entered New PIN");
        cin >> PIN1;
        cout << "Re-enter New PIN: ";
        LOG(" User Re-entered New PIN");
        cin >> PIN2;
        if (PIN1==PIN2) {
            LOG(" User Confirmed New PIN");
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
            LOG(" PIN File Updated");
            cout << "PIN changed successfully!" << endl;
        }
        else {
            LOG("  User Entered Different PINs");
            cout << "New PINs do not match!" << endl;
        }
    }
    else {
        LOG("  User Entered Wrong PIN");
        cout << "Enterd PIN is incorrect" << endl;
    }

}//LOG DONE
void System::Transactions() {
    cout << "Last Transactions: " << endl;
    string Key = account.getkey();
    LOG(" Transactions Started");
    for (int i = 0; i < 95; i++) {
        string Transaction = account.displayTransactions(i);
        if (Transaction != "") {
            Transaction = decrypt(Transaction, Key);
            cout << i+1 << "] "<<Transaction << endl;
        }
    }
    LOG(" Transactions Displayed");
}//LOG DONE
void System::Logout() {
    account.endAccount();
    LOG(" Logout Started");
    cout << "Logging Out" << endl;
    loggedIn=0;
    LOG(" Logout Finished");

}//LOG DONE

void System::LOG(string Text)
{
    string Key = MainSystemKey;
    time_t currentTime = time(0);

    char* dt = ctime(&currentTime);
    string LOG = Text+ " " + " : " +dt;
    LOG = encrypt(LOG, Key);

    ofstream file("../Data/LOG", std::ios::app);
    if (file.is_open()) {
        file << LOG << endl;
        file.close();
    }
    else {
        cerr << "Unable to open log file" << endl;
    }

}//LOG NO

void System::AdminMenu() {
    LOG(" Admin Menu Started");
    cout << "Admin Mode, Welcome to ATM" << endl;
    cout << "Session Number: "<< SessionNumber << endl;
    cout << "1. Add New Account" << endl;
    cout << "2. Remove Account" << endl;
    cout << "3. Show All Accounts" << endl;
    cout << "4. Show LOGS" << endl;
    cout << "5. Change Main Key" << endl;
    cout << "6. Exit Admin Mode" << endl;
    int choice;
    cin >> choice;
    LOG(" Admin Made a choice");
    switch (choice) {
        case 1: {
                LOG(" Admin Chose Add Account");
                AdminAddAccount();
                AdminMenu();
                break;
        }
        case 2:
        {
                LOG(" Admin Chose Remove Account");
                AdminRemoveAccount();
                AdminMenu();
                break;
        }

        case 3:
            {
                LOG(" Admin Chose Show All Accounts");
                AdminShowAllAccounts();
                AdminMenu();
                break;
            }
        case 4:
        {
                LOG(" Admin Chose Show Logs");
                AdminShowLogs();
                AdminMenu();
                break;
        }
        case 5:
        {
                LOG(" Admin Chose Change Key");
                AdminChangeKey();
                AdminMenu();
                break;
        }
        case 6:
        {
                LOG(" Admin Chose Exit Admin Mode");
                ListNum=-1;
                break;
        }
    }
}//LOG DONE
void System::AdminAddAccount()
{
    cout << "Current Accounts: " << NumberOfAccounts <<endl;
    cout << "Please Enter Account Number: (enter 0 to cancel)" << endl;
    string AccNum;
    cin >> AccNum;
    LOG(" User Entered Account Number");
    if (AccNum == "0")
    {
        LOG("  User Canceled Add Account");
        return;
    }
    //check if account number is already in use
    ifstream filex("../Login/LoginData/LD1.data");
    string ARRNames[100];
    LOG(" Add Account File Opened");
    for (int i= 0; i < 100; i++) {
        string temp;
        filex >> temp;
        temp= decrypt(temp, MainSystemKey);
        ARRNames[i]=temp;
        if (filex.eof()) {
            break;
            LOG(" Add Account File Moved to Array");
        }
    }
    for (int i= 0; i < 100; i++) {
        if (AccNum == ARRNames[i]) {
            cout << "Account number already in use. Please try again." << endl;
            LOG(" User Entered Account Number Already in Use");
            return;
        }
    }
    cout << "Please Enter PIN: ";
    string AccPIN;
    cin >> AccPIN;
    LOG(" User Entered PIN");
    cout << "Please Enter First Name: ";
    string FName;
    cin >> FName;
    LOG(" User Entered First Name");
    cout << "Please Enter Middle Name: ";
    string MName;
    cin >> MName;
    LOG(" User Entered Middle Name");
    cout << "Please Enter Last Name: ";
    string LName;
    cin >> LName;
    LOG(" User Entered Last Name");
    string Name= FName + " " + MName + " " + LName;
    cout << "Please Enter Balance: ";
    string Balance;
    cin >> Balance;
    LOG(" User Entered Balance");

    string Key = generate_random_key();
    string EncAccNum = encrypt(AccNum, Key);
    string EncPIN = encrypt(AccPIN, Key);
    string EncName = encrypt(Name, Key);
    string EncBalance = encrypt(Balance, Key);
    LOG(" Account Data Encrypted");


    cout << "Current Accounts: " << NumberOfAccounts << endl;
    int temp= stoi(NumberOfAccounts);
    temp++;
    NumberOfAccounts = to_string(temp);
    LOG(" Number of Accounts Incremented");

    string filelocation = "../Data/U"+(NumberOfAccounts)+".data";
    ofstream file(filelocation);
    if (file.is_open()) {
        file << Key << endl;
        file << EncAccNum << endl;
        file << EncName << endl;
        file << EncBalance << endl;
        cout << "Account data saved successfully." << endl;
        LOG(" Account Data Saved");
        file.close();
    }
    else {
        cerr << "Unable to open new file" << endl;
        LOG(" Failed to save Account Data");
    }

    ofstream file2("../Login/LoginData/LD1.data", std::ios::app);
    if (file2.is_open()) {
        file2 << encrypt(AccNum, MainSystemKey) << endl;
        file2.close();
        LOG(" LD1 File Updated");
    }
    else {
        cerr << "Unable to open LD1 file" << endl;
        LOG(" Failed to update LD1 file");
    }

    ofstream file3("../Login/LoginData/LD2.data", std::ios::app);
    if (file3.is_open()) {
        LOG(" LD2 File Opened");
        file3 << encrypt(AccPIN, MainSystemKey) << endl;
    }
    else {
        LOG(" Failed to update LD2 file");
        cerr << "Unable to open LD2 file" << endl;
    }


    ofstream MainFile("../Data/Don't Open this!");
    if (MainFile.is_open()) {
        MainFile << MainSystemKey << endl;
        MainFile << NumberOfAccounts << endl;
        cout << "Main file updated successfully." << endl;
        MainFile.close();
        LOG(" Main File Updated");
    }
    else {
        LOG(" Failed to update Main file");
        cerr << "Unable to open main file" << endl;
    }

}//LOG DONE
void System::AdminRemoveAccount()
{
    cout << "Current Accounts: " << NumberOfAccounts <<endl;
    ifstream file("../Login/LoginData/LD1.data");
    LOG(" Remove Account File Opened");
    string ARRNames[100];
    for (int i= 0; i < 100; i++) {
        string temp;
        file >> temp;
        temp= decrypt(temp, MainSystemKey);
        ARRNames[i]=temp;
        cout << i << "] " << temp << endl;
        if (file.eof()) {
            break;
        }
    }
    cout << "Please Choose Account: (Enter 0 to cancel)" << endl;
    int target;
    cin >> target;
    LOG(" User Chose Account");
    if (target == 0)
    {
        LOG(" User Canceled Remove Account");
        return;
    }
    if (target<1 || target>100) {
        cout << "Invalid choice. Please try again." << endl;
        LOG(" User Gave Invalid choice");
        return;
    }

    string filename= "../Data/U"+ to_string(target) +".data";
    ofstream targetfile(filename);
    LOG(" Target File Opened");
    if (targetfile.is_open()) {
        targetfile << "Deleted" << endl;
        cout << "Account data deleted successfully." << endl;
        targetfile.close();
        LOG(" Target File Deleted");
    }
    else {
        cerr << "Unable to open target file" << endl;
        LOG(" Failed to delete Target file");
    }
    ofstream MainFile("../Data/Don't Open this!");
    if (MainFile.is_open()) {
        LOG(" Main File Opened");
        MainFile << MainSystemKey << endl;
        MainFile << NumberOfAccounts << endl;
        cout << "Main file updated successfully." << endl;
        MainFile.close();
        LOG(" Main File Updated");
    }
    else {
        LOG(" Failed to update Main file");
        cerr << "Unable to open main file" << endl;
    }


    LOG(" LD1 File Opened");
    ifstream file2("../Login/LoginData/LD1.data");
    string ARR1[100];
    for (int i = 0; i < 100; i++) {
        string temp;
        file2 >> temp;
        ARR1[i]=temp;
        if (file2.eof()) {
            break;
        }
    }
    LOG(" LD1 File Moved to Array");
    LOG(" LD1 File Opened");
    ofstream file2_out("../Login/LoginData/LD1.data");
    if (file2_out.is_open()) {
        for (int i = 0; i < 100; i++) {
            if (i == target) {
                continue; // Skip the deleted account
            }
            if (!ARR1[i].empty()) {
                file2_out << ARR1[i] << endl;
            }
        }
        LOG(" LD1 File Updated");
        file2_out.close();
        cout << "Data in file2 updated successfully." << endl;
        LOG(" LD1 File Updated");
    }
    else
    {
        LOG(" Failed to update LD1 file");
        cerr << "Unable to open file2 for writing" << endl;
    }
    LOG(" LD2 File Opened");
    ifstream file3("../Login/LoginData/LD2.data");
    string ARR2[100];
    for (int i = 0; i < 100; i++) {
        string temp;
        file3 >> temp;
        ARR2[i]=temp;
        if (file3.eof()) {
            break;
        }
    }
    LOG(" LD2 File Moved to Array");

    ofstream file3_out("../Login/LoginData/LD2.data");
    if (file3_out.is_open()) {
        for (int i = 0; i < 100; i++) {
            if (i == target) {
                continue; // Skip the deleted account
            }
            if (!ARR2[i].empty()) {
                file3_out << ARR2[i] << endl;
            }
        }
        LOG(" LD2 File Updated");
        file3_out.close();
        cout << "Data in file3 updated successfully." << endl;
    }
    else
    {
        LOG(" Failed to update LD2 file");
        cerr << "Unable to open file3 for writing" << endl;
    }
    LOG(" Remove Account Finished");



}//LOG DONE
void System::AdminShowAllAccounts()
{
    ifstream file("../Login/LoginData/LD1.data");
    string ARRNames[100];
    LOG(" Show All Accounts File Opened");
    for (int i= 0; i < 100; i++) {
        string temp;
        file >> temp;
        temp= decrypt(temp, MainSystemKey);
        ARRNames[i]=temp;
        cout << i << "] " << temp << endl;
        if (file.eof()) {
            break;
            LOG(" Show All Accounts File Moved to Array");
        }
    }
}//LOG DONE
void System::AdminShowLogs()
{
    ifstream file("../Data/LOG");
    string temp;
    LOG(" Show Logs File Opened");
    string Key = MainSystemKey;
    for (int i= 0; i < 100000; i++) {
        string temp;
        file >> temp;
        temp= decrypt(temp, Key);
        cout << i+1 << "] " << temp << endl;
        if (file.eof()) {
            LOG( " Show Logs File Moved to Array");
            break;
        }
    }
}//LOG DONE
void System::AdminChangeKey()
{
    cout << "Please Enter New Key: (Enter 0 to cancel)" << endl;
    string NewKey;
    if (NewKey == "0")
    LOG(" User Entered New Key");
    string oldKey = MainSystemKey;
    cin >> NewKey;
    if (NewKey == "0")
    {
        LOG(" User Canceled Change Key");
        return;
    }
    LOG(" User Entered New Key");
    // Update the main system key
    MainSystemKey = NewKey;
    LOG(" Main Key Set");
    // Update the main file with the new key
    ofstream MainFile("../Data/Don't Open this!");
    if (MainFile.is_open()) {
        MainFile << MainSystemKey << endl;
        MainFile << NumberOfAccounts << endl;
        cout << "Main file updated successfully." << endl;
        MainFile.close();
        LOG(" Main File Updated");
    } else {
        cerr << "Unable to open main file for writing." << endl;
        return;
        LOG(" Failed to update Main file");
    }

    //Change all log file data
    ifstream logFile("../Data/LOG");
    if (!logFile.is_open()) {
        cerr << "Unable to open log file for reading." << endl;
        return;
        LOG(" Failed to open Log file");
    }
    vector<string> updatedLogs;
    string temp;
    LOG(" Log File Opened");
    while (logFile >> temp) {
        string decryptedData = decrypt(temp, oldKey);
        string encryptedData = encrypt(decryptedData, MainSystemKey);
        updatedLogs.push_back(encryptedData);
        LOG(" Log File Moved to Array");
    }
    logFile.close();
    LOG(" Log File Closed");
    ofstream logFile_out("../Data/LOG");
    if (logFile_out.is_open()) {
        for (const auto& data : updatedLogs) {
            logFile_out << data << endl;
            LOG(" Log File Updated");
        }
        logFile_out.close();
        cout << "Log file updated successfully." << endl;
        LOG(" Log File Updated");
    } else {
        cerr << "Unable to open log file for writing." << endl;
        return;
        LOG(" Failed to update Log file");
    }



    // Process LD1.data
    ifstream file1("../Login/LoginData/LD1.data");
    if (!file1.is_open()) {
        cerr << "Unable to open LD1 file for reading." << endl;
        return;
        LOG(" Failed to open LD1 file");
    }
    vector<string> updatedLD1;
    string temp1;
    LOG(" LD1 File Opened");
    while (file1 >> temp1) {
        string decryptedData = decrypt(temp1, oldKey);
        string encryptedData = encrypt(decryptedData, MainSystemKey);
        updatedLD1.push_back(encryptedData);
        LOG(" LD1 File Moved to Array");
    }
    file1.close();
    LOG(" LD1 File Closed");

    ofstream file1_out("../Login/LoginData/LD1.data");
    if (file1_out.is_open()) {
        for (const auto& data : updatedLD1) {
            file1_out << data << endl;
            LOG(" LD1 File Updated");
        }
        file1_out.close();
        cout << "LD1 file updated successfully." << endl;
        LOG(" LD1 File Updated");
    } else {
        cerr << "Unable to open LD1 file for writing." << endl;
        LOG(" Failed to update LD1 file");
        return;
    }

    // Process LD2.data
    ifstream file2("../Login/LoginData/LD2.data");
    if (!file2.is_open()) {
        LOG(" Failed to open LD2 file");
        cerr << "Unable to open LD2 file for reading." << endl;
        return;
    }
    LOG(" LD2 File Opened");
    vector<string> updatedLD2;
    while (file2 >> temp) {
        string decryptedData = decrypt(temp, oldKey);
        string encryptedData = encrypt(decryptedData, MainSystemKey);
        updatedLD2.push_back(encryptedData);
    }
    file2.close();
    LOG(" LD2 File Closed");

    ofstream file2_out("../Login/LoginData/LD2.data");
    if (file2_out.is_open()) {
        for (const auto& data : updatedLD2) {
            file2_out << data << endl;
            LOG(" LD2 File Updated");
        }
        file2_out.close();
        cout << "LD2 file updated successfully." << endl;
    } else {
        cerr << "Unable to open LD2 file for writing." << endl;
        return;
        LOG(" Failed to update LD2 file");
    }

    cout << "System key changed and all data updated successfully." << endl;
    LOG(" System Key Changed");
}//LOG DONE

string System::generate_random_key() {

    int key_length = 20;
    string character_set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    if (character_set.empty()) {
        return "";
    }

    // Seed the random number generator
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> distribution(0, character_set.size() - 1);

    // Generate the random key
    string key;
    for (int i = 0; i < key_length; ++i) {
        key += character_set[distribution(generator)];
    }
    LOG(" Random Key Generated");
    return key;
}//LOG DONE