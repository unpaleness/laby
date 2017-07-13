#include "laby.h"

/****************\
* PUBLIC METHODS *
\****************/

Laby::Laby() {}

Laby::Laby(int x, int y, ofstream *l, ostream *m) : x(x), y(y), log_stream(l), main_stream(m) {
    if (x > 0 && y > 0) {
        walls_v = new bool [y * (x + 1)];
        walls_h = new bool [(y + 1) * x];
        *log_stream << "Info: " << __FUNCTION__ << "(): construction complete" << endl;
        generate();
    } else {
        *log_stream << "Warning: " << __FUNCTION__ << "(): walls not initialized" << endl;
    }
}

Laby::~Laby() {
    if (walls_v != nullptr) {
        *log_stream << "Info: " << __FUNCTION__ << "(): deleting vertical walls" << endl;
        delete [] walls_v;
    }
    if (walls_h != nullptr) {
        *log_stream << "Info: " << __FUNCTION__ << "(): deleting horizontal walls" << endl;
        delete [] walls_h;
    }
    *log_stream << "Info: " << __FUNCTION__ << "(): destruction complete" << endl;
}

int Laby::print() {
    if (walls_v == nullptr || walls_h == nullptr) {
        *log_stream << "Warning: " << __FUNCTION__ << "(): walls not initialized" << endl;
        return 1;
    }
    *log_stream << "Info: " << __FUNCTION__ << "(): printing" << endl;
    for (int j = 0; j < y + 1; ++j) {
        // Upper corners and upper borders
        for (int i = 0; i < x; ++i) {
            *main_stream << '#';
            if (walls_h[j * x + i]) {
                *main_stream << '#';
            } else {
                *main_stream << ' ';
            }
        }
        // Right upper corner of the last cell in row
        *main_stream << '#';
        if (j < y) {
            // Left borders and cells itself (empty)
            for (int i = 0; i < x + 1; ++i) {
                if (i > 0) {
                    *main_stream << ' ';
                }
                if (walls_v[j * (x + 1) + i]) {
                    *main_stream << '#';
                } else {
                    *main_stream << ' ';
                }
            }
        }
        *main_stream << endl;
    }
    return 0;
}

/****************\
* PUBLIC METHODS *
\****************/

int Laby::generate() {
    if (walls_v == nullptr || walls_h == nullptr) {
        *log_stream << "Warning: " << __FUNCTION__ << "(): cannot generate, walls not initialized" << endl;
        return 1;
    }
    *log_stream << "Info: " << __FUNCTION__ << "(): generating" << endl;
    return 0;
}
