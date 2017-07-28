#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>

using namespace std;

#include "laby.h"

using namespace Labyrinth;

int main(int argc, char **argv) {
    string name     { argv[0] };
    string log_name { "laby.log" };
    int x           { 0 };
    int y           { 0 };
    int iter_limit  { 0 };
    if (argc > 4) {
        log_name   = argv[1];
        x          = atoi(argv[2]);
        y          = atoi(argv[3]);
        iter_limit = atoi(argv[4]);
    } else {
        cerr << "Too few arguments" << endl;
        cerr << "1 argument: log file name" << endl;
        cerr << "2 argument: width" << endl;
        cerr << "3 argument: height" << endl;
        cerr << "4 argument: iterations limit" << endl;
        return 1;
    }

    srand(time(0));

    ofstream *log = nullptr;
    if (!log_name.empty()) {
        log = new ofstream (log_name);
    }

    Laby *laby = nullptr;
    laby = new Laby (x, y, iter_limit, log, &cout);
    laby->solve();
    laby->print();

    if (laby != nullptr) {
        delete laby;
    }
    if (log != nullptr) {
        delete log;
    }
    return 0;
}
