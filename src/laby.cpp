#include <cstdlib>
#include <set>
#include <map>

#include "laby.h"

/****************\
* PUBLIC METHODS *
\****************/

Laby::Laby() {}

Laby::Laby(int x, int y, ofstream *l, ostream *m) : x(x), y(y), log_stream(l), main_stream(m) {
    *log_stream << "Info: " << __FUNCTION__ << "(): constructing..." << endl;
    if (x > 0 && y > 0) {
        // end.x = x - 1;
        // end.y = y - 1;
        // *main_stream << 1 << endl;
        *main_stream << x << endl << y << endl;
        walls_v = new bool *[y];
        for (int i = 0; i < y; ++i) {
            walls_v[i] = new bool [x + 1];
        }
        walls_h = new bool *[y + 1];
        for (int i = 0; i < y + 1; ++i) {
            walls_h[i] = new bool [x];
        }
        // *main_stream << 2 << endl;
        // *log_stream << "Info: " << __FUNCTION__ << "(): begin = (" << begin.x << ';' << begin.y << ')' << endl;
        // *log_stream << "Info: " << __FUNCTION__ << "(): end = (" << end.x << ';' << end.y << ')' << endl;
        *log_stream << "Info: " << __FUNCTION__ << "(): construction complete" << endl;
        generate();
        solve();
    } else {
        *log_stream << "Warning: " << __FUNCTION__ << "(): walls not initialized" << endl;
    }
}

Laby::~Laby() {
    *log_stream << "Info: " << __FUNCTION__ << "(): destructing..." << endl;
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
    *log_stream << "Info: " << __FUNCTION__ << "(): printing" << endl;
    if (walls_v == nullptr || walls_h == nullptr) {
        *log_stream << "Warning: " << __FUNCTION__ << "(): walls not initialized" << endl;
        return 1;
    }
    for (int j = 0; j < y + 1; ++j) {
        // Upper corners and upper borders
        for (int i = 0; i < x; ++i) {
            *main_stream << "\u2588";
            if (walls_h[j][i]) {
                *main_stream << "\u2588";
            } else {
                *main_stream << ' ';
            }
        }
        // Right upper corner of the last cell in row
        *main_stream << "\u2588";
        *main_stream << endl;
        if (j < y) {
            // Left borders and cells itself (empty)
            for (int i = 0; i < x + 1; ++i) {
                if (i > 0) {
                    *main_stream << ' ';
                }
                if (walls_v[j][i]) {
                    *main_stream << "\u2588";
                } else {
                    *main_stream << ' ';
                }
            }
            *main_stream << endl;
        }
    }
    *log_stream << "Info: " << __FUNCTION__ << "(): printing complete" << endl;
    return 0;
}

/*****************\
* PRIVATE METHODS *
\*****************/

int Laby::generate() {
    *log_stream << "Info: " << __FUNCTION__ << "(): generating..." << endl;
    if (walls_v == nullptr || walls_h == nullptr) {
        *log_stream << "Warning: " << __FUNCTION__ << "(): cannot generate, walls not initialized" << endl;
        return 1;
    }

    // Edge borders
    for (int i = 0; i < x; ++i) {
        walls_h[0][i] = true;
        walls_h[y][i] = true;
    }
    for (int i = 0; i < y; ++i) {
        walls_v[i][0] = true;
        walls_v[i][x] = true;
    }
    // In this case generation is not needed
    if (x < 2 || y < 2) {
        *log_stream << "Info: " << __FUNCTION__ << "(): generating complete (too small)" << endl;
        return 0;
    }
    /***********\
    * First row *
    \***********/
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
    // If amount of lower borders is equal to cells of each multi delete one
    // single random lower border of each multi
    for (auto m : multies) {
        if (multi_borders[m] == multi_cells[m]) {
            int i_deleting_border { rand() % multi_borders[m] + 1 };
            int i_border { 0 };
            for (int i = 0; i < x; ++i) {
                if (row[i] == m) {
                    ++i_border;
                }
                if (i_border == i_deleting_border) {
                    walls_h[1][i] = false;
                    break;
                }
            }
        }
    }
    /**************\
    * 2 - y-1 rows *
    \**************/
    for (int j = 1; j < y - 1; ++j) {
        // Set new multies to the cells with borders above
        for (int i = 0; i < x; ++i) {
            if (walls_h[j][i]) {
                row[i] = ++n_multi;
            }
        }
        // Set vertical borders
        for (int i = 0; i < x - 1; ++i) {
            if (row[i] == row[i + 1]) {
                walls_v[j][i + 1] = true;
            } else {
                walls_v[j][i + 1] = rand_bool();
                // All cells with the same multi should be changed
                if (!walls_v[j][i + 1]) {
                    int multi_to_merge { row[i + 1] };
                    for (int k = 0; k < x; ++k) {
                        if (row[k] == multi_to_merge) {
                            row[k] = row[i];
                        }
                    }
                }
            }
        }
        // Set horizontal borders
        for (int i = 0; i < x; ++i) {
            walls_h[j + 1][i] = rand_bool();
        }
        multies.clear();
        for (int i = 0; i < x; ++i) {
            multies.insert(row[i]);
        }
        for (auto m : multies) {
            multi_cells[m] = 0;
            multi_borders[m] = 0;
        }
        // Count cells and lower borders for each multi
        for (int i = 0; i < x; ++i) {
            ++multi_cells[row[i]];
            if (walls_h[j + 1][i]) {
                ++multi_borders[row[i]];
            }
        }
        // If amount of lower borders as equal to cells of each multi delete one
        // single random lower border of each multi
        for (auto m : multies) {
            if (multi_borders[m] == multi_cells[m]) {
                int i_deleting_border { rand() % multi_borders[m] + 1 };
                int i_border { 0 };
                for (int i = 0; i < x; ++i) {
                    if (row[i] == m) {
                        ++i_border;
                    }
                    if (i_border == i_deleting_border) {
                        walls_h[j + 1][i] = false;
                        break;
                    }
                }
            }
        }
    }
    /**************\
    * The last row *
    \**************/
    // Set new multies to the cells with borders above
    for (int i = 0; i < x; ++i) {
        if (walls_h[y - 1][i]) {
            row[i] = ++n_multi;
        }
    }
    // Set vertical borders
    for (int i = 0; i < x - 1; ++i) {
        if (row[i] == row[i + 1]) {
            walls_v[y - 1][i + 1] = true;
        } else {
            walls_v[y - 1][i + 1] = rand_bool();
            // All cells with the same multi should be changed
            if (!walls_v[y - 1][i + 1]) {
                int multi_to_merge { row[i + 1] };
                for (int k = 0; k < x; ++k) {
                    if (row[k] == multi_to_merge) {
                        row[k] = row[i];
                    }
                }
            }
        }
    }
    // Remove borders between different multies and merge them
    for (int i = 0; i < x - 1; ++i) {
        if (row[i] != row[i + 1]) {
            walls_v[y - 1][i + 1] = false;
            int multi_to_merge { row[i + 1] };
            for (int k = i + 1; k < x; ++k) {
                if (row[k] == multi_to_merge) {
                    row[k] = row[i];
                }
            }
        }
    }
    *log_stream << "Info: " << __FUNCTION__ << "(): generating complete" << endl;
    return 0;
}

int Laby::solve() {
    *log_stream << "Info: " << __FUNCTION__ << "(): solving..." << endl;
    if (walls_v == nullptr || walls_h == nullptr) {
        *log_stream << "Warning: " << __FUNCTION__ << "(): cannot solve, walls not initialized" << endl;
        return 1;
    }
    *log_stream << "Info: " << __FUNCTION__ << "(): solving complete" << endl;
    return 0;
}

bool Laby::rand_bool() {
    return rand() % 2 ? true : false;
}
