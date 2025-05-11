#include "System Class/System.h"

int main() {

    cout << "Please Press 1 to Start or 0 to End" << endl;
    int start;
    cin >> start;
    if (start == 1) {
        System system;
        system.run();
    }
    else if (start == 0) {

    }
    else {
        main();
    }

    return 0;
}