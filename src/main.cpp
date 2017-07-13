#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

using namespace std;

#include "laby.h"

int main(int argc, char **argv) {
    string name { argv[0] };
    string log_name;
    if (argc > 1) {
        log_name = argv[1];
    } else {
        cerr << "Too few arguments" << endl;
        cerr << "1 argument: log file name" << endl;
        return 1;
    }

    srand(time(0));

    ofstream *log = nullptr;
    if (!log_name.empty()) {
        log = new ofstream (log_name);
    }

    Laby *laby = nullptr;
    laby = new Laby (8, 8, log, &cout);
    laby->print();

    if (laby != nullptr) {
        delete laby;
    }
    if (log != nullptr) {
        delete log;
    }
    return 0;
}
