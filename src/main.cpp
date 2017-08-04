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
    int start_x     { 0 };
    int start_y     { 0 };
    int end_x       { 0 };
    int end_y       { 0 };
    if (argc > 8) {
        log_name   = argv[1];
        x          = atoi(argv[2]);
        y          = atoi(argv[3]);
        iter_limit = atoi(argv[4]);
        start_x    = atoi(argv[5]);
        start_y    = atoi(argv[6]);
        end_x      = atoi(argv[7]);
        end_y      = atoi(argv[8]);
    } else {
        cerr << "Too few arguments" << endl;
        cerr << "1 argument: log file name" << endl;
        cerr << "2 argument: width" << endl;
        cerr << "3 argument: height" << endl;
        cerr << "4 argument: iterations limit" << endl;
        cerr << "5 argument: start point x" << endl;
        cerr << "6 argument: start point y" << endl;
        cerr << "7 argument: end point x" << endl;
        cerr << "8 argument: end point y" << endl;
        return 1;
    }

    srand(time(0));

    ofstream *log = nullptr;
    if (!log_name.empty()) {
        log = new ofstream (log_name);
    }

    Laby *laby = nullptr;
    laby = new Laby (log, &cout, x, y, iter_limit, start_x, start_y, end_x, end_y);
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
