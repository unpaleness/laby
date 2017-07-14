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
    map <int, string> nodes;
    nodes[0]  = "\u250c"; // Left upper corner
    nodes[1]  = "\u2510"; // Right upper corner
    nodes[2]  = "\u2514"; // Left lower corner
    nodes[3]  = "\u2518"; // Right lower corner
    nodes[4]  = "\u251c"; // Left T-element
    nodes[5]  = "\u2524"; // Right T-element
    nodes[6]  = "\u252c"; // Upper T-element
    nodes[7]  = "\u2534"; // Lower T-element
    nodes[8]  = "\u253c"; // Cross
    nodes[9]  = "\u2502"; // Vertical stick
    nodes[10] = "\u2500"; // Horisontal stick
    nodes[11] = "\u2575"; // Vertical upper half-stick
    nodes[12] = "\u2577"; // Vertical lower half-stick
    nodes[13] = "\u2574"; // Horisontal left half-stick
    nodes[14] = "\u2576"; // Horisontal right half-stick
    nodes[15] = " "; // Space
    if (x < 2 || y < 2) {
        *main_stream << nodes[0];
        for (int i = 1; i < x - 1; ++i) {
            *main_stream << nodes[10];
        }
        *main_stream << nodes[1] << endl;
        for (int j = 1; j < y - 1; ++j) {
            *main_stream << nodes[9] << nodes[9] << endl;
        }
        *main_stream << nodes[2];
        for (int i = 1; i < x - 1; ++i) {
            *main_stream << nodes[10];
        }
        *main_stream << nodes[3] << endl;
    } else {
        *main_stream << nodes[0];
        for (int i = 1; i < x; ++i) {
            if (walls_v[0][i]) {
                *main_stream << nodes[6];
            } else {
                *main_stream << nodes[10];
            }
        }
        *main_stream << nodes[1] << endl;
        for (int j = 1; j < y; ++j) {
            if (walls_h[j][0]) {
                *main_stream << nodes[4];
            } else {
                *main_stream << nodes[9];
            }
            for (int i = 1; i < x; ++i) {
                if (walls_v[j - 1][i] && walls_v[j][i] && walls_h[j][i - 1] && walls_h[j][i])
                    *main_stream << nodes[8];
                if (!walls_v[j - 1][i] && walls_v[j][i] && walls_h[j][i - 1] && walls_h[j][i])
                    *main_stream << nodes[6];
                if (walls_v[j - 1][i] && !walls_v[j][i] && walls_h[j][i - 1] && walls_h[j][i])
                    *main_stream << nodes[7];
                if (walls_v[j - 1][i] && walls_v[j][i] && !walls_h[j][i - 1] && walls_h[j][i])
                    *main_stream << nodes[4];
                if (walls_v[j - 1][i] && walls_v[j][i] && walls_h[j][i - 1] && !walls_h[j][i])
                    *main_stream << nodes[5];
                if (!walls_v[j - 1][i] && !walls_v[j][i] && walls_h[j][i - 1] && walls_h[j][i])
                    *main_stream << nodes[10];
                if (!walls_v[j - 1][i] && walls_v[j][i] && !walls_h[j][i - 1] && walls_h[j][i])
                    *main_stream << nodes[0];
                if (!walls_v[j - 1][i] && walls_v[j][i] && walls_h[j][i - 1] && !walls_h[j][i])
                    *main_stream << nodes[1];
                if (walls_v[j - 1][i] && !walls_v[j][i] && !walls_h[j][i - 1] && walls_h[j][i])
                    *main_stream << nodes[2];
                if (walls_v[j - 1][i] && !walls_v[j][i] && walls_h[j][i - 1] && !walls_h[j][i])
                    *main_stream << nodes[3];
                if (walls_v[j - 1][i] && walls_v[j][i] && !walls_h[j][i - 1] && !walls_h[j][i])
                    *main_stream << nodes[9];
                if (!walls_v[j - 1][i] && !walls_v[j][i] && !walls_h[j][i - 1] && walls_h[j][i])
                    *main_stream << nodes[14];
                if (!walls_v[j - 1][i] && !walls_v[j][i] && walls_h[j][i - 1] && !walls_h[j][i])
                    *main_stream << nodes[13];
                if (!walls_v[j - 1][i] && walls_v[j][i] && !walls_h[j][i - 1] && !walls_h[j][i])
                    *main_stream << nodes[12];
                if (walls_v[j - 1][i] && !walls_v[j][i] && !walls_h[j][i - 1] && !walls_h[j][i])
                    *main_stream << nodes[11];
                if (!walls_v[j - 1][i] && !walls_v[j][i] && !walls_h[j][i - 1] && !walls_h[j][i])
                    *main_stream << nodes[15];
            }
            if (walls_h[j][x - 1]) {
                *main_stream << nodes[5] << endl;
            } else {
                *main_stream << nodes[9] << endl;
            }
        }
        *main_stream << nodes[2];
        for (int i = 1; i < x; ++i) {
            if (walls_v[y - 1][i]) {
                *main_stream << nodes[7];
            } else {
                *main_stream << nodes[10];
            }
        }
        *main_stream << nodes[3] << endl;
    }
    *log_stream << "Info: " << __FUNCTION__ << "(): printing (" << x << 'x' << y << ") complete" << endl;
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

bool Laby::rand_bool() {
    return rand() % 2 ? true : false;
}
