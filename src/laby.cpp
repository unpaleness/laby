#include <cstdlib>
#include <set>
#include <map>

#include "laby.h"

/****************\
* PUBLIC METHODS *
\****************/

Laby::Laby() {}

Laby::Laby(int x, int y, ofstream *l, ostream *m) : x(x), y(y), log_stream(l), main_stream(m) {
    if (x > 0 && y > 0) {
        walls_v = new bool *[y];
        for (int i = 0; i < y; ++i) {
            walls_v[i] = new bool [x + 1];
        }
        walls_h = new bool *[y + 1];
        for (int i = 0; i < y + 1; ++i) {
            walls_h[i] = new bool [x];
        }
        *log_stream << "Info: " << __FUNCTION__ << "(): construction complete" << endl;
        generate();
    } else {
        *log_stream << "Warning: " << __FUNCTION__ << "(): walls not initialized" << endl;
    }
}

Laby::~Laby() {
    if (walls_v != nullptr) {
        *log_stream << "Info: " << __FUNCTION__ << "(): deleting vertical walls" << endl;
        for (int i = 0; i < y; ++i) {
            delete [] walls_v[i];
        }
        delete [] walls_v;
    }
    if (walls_h != nullptr) {
        *log_stream << "Info: " << __FUNCTION__ << "(): deleting horizontal walls" << endl;
        for (int i = 0; i < y + 1; ++i) {
            delete [] walls_h[i];
        }
        delete [] walls_h;
    }
    if (log_stream != nullptr) {
        *log_stream << "Info: " << __FUNCTION__ << "(): destruction complete" << endl;
    }
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
            if (walls_h[j][i]) {
                *main_stream << '#';
            } else {
                *main_stream << ' ';
            }
        }
        // Right upper corner of the last cell in row
        *main_stream << '#';
        *main_stream << endl;
        if (j < y) {
            // Left borders and cells itself (empty)
            for (int i = 0; i < x + 1; ++i) {
                if (i > 0) {
                    *main_stream << ' ';
                }
                if (walls_v[j][i]) {
                    *main_stream << '#';
                } else {
                    *main_stream << ' ';
                }
            }
            *main_stream << endl;
        }
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

    // Edge borders
    for (int i = 0; i < x; ++i) {
        walls_h[0][i] = true;
        walls_h[y][i] = true;
    }
    for (int i = 0; i < y; ++i) {
        walls_v[i][0] = true;
        walls_v[i][x] = true;
    }
    // First row
    int row[x];
    int n_multi { 1 }; // Index of the last multiplicity created
    for (int i = 0; i < x; ++i) {
        if (i > 0) {
            walls_v[0][i] = rand_bool();
            if (walls_v[0][i]) {
                ++n_multi;
            }
        }
        walls_h[1][i] = rand_bool();
        row[i] = n_multi;
    }
    *main_stream << '#';
    for (int i = 0; i < x; ++i) {
        *main_stream << row[i];
        if (walls_v[0][i + 1]) {
            *main_stream << '#';
        } else {
            *main_stream << ' ';
        }
    }
    *main_stream << endl << '#';
    for (int i = 0; i < x; ++i) {
        if (walls_h[1][i]) {
            *main_stream << '#';
        } else {
            *main_stream << ' ';
        }
        *main_stream << '#';
    }
    *main_stream << endl;
    set <int> multies (row, row + x); // A set of multies on current row
    map <int, int> multi_cells;       // Amount of cells of each multi
    map <int, int> multi_borders;     // Amount of lower borders of each multi
    for (auto m : multies) {
        multi_cells[m] = 0;
        multi_borders[m] = 0;
    }
    // Count cells and lower borders for each multi
    for (int i = 0; i < x; ++i) {
        ++multi_cells[row[i]];
        if (walls_h[1][i]) {
            ++multi_borders[row[i]];
        }
    }
    // If amount of lower borders as equal to cells of each multi delete one
    // single random lower border of each multi
    for (auto m : multies) {
        *main_stream << m << ' ' << multi_cells[m] << ' ' << multi_borders[m] << endl;
        if (multi_borders[m] == multi_cells[m]) {
            *main_stream << "Multiplicity " << m << " is now trapped" << endl;
            int i_deleting_border { rand() % multi_borders[m] + 1 };
            int i_border { 0 };
            for (int i = 0; i < x; ++i) {
                if (row[i] == m) {
                    ++i_border;
                }
                if (i_border == i_deleting_border) {
                    walls_h[1][i] = false;
                    *main_stream << "Deleting wall under " << i + 1 << " cell" << endl;
                    break;
                }
            }
        }
    }

    return 0;
}

bool Laby::rand_bool() {
    return rand() % 2 ? true : false;
}
