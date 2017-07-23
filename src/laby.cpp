#include <cstdlib>
#include <set>

#include "laby.h"

/****************\
* PUBLIC METHODS *
\****************/

Laby::Laby() {}

Laby::Laby(int x, int y, ofstream *l, ostream *m) : x(x), y(y), log_stream(l), main_stream(m) {
    *log_stream << "Info: " << __FUNCTION__ << "(): constructing..." << endl;
    if (x > 0 && y > 0) {
        end_cell.x = x - 1;
        end_cell.y = y - 1;
        walls_v = new bool *[y];
        for (int i = 0; i < y; ++i) {
            walls_v[i] = new bool [x + 1];
        }
        walls_h = new bool *[y + 1];
        for (int i = 0; i < y + 1; ++i) {
            walls_h[i] = new bool [x];
        }
        *log_stream << "Info: " << __FUNCTION__ << "(): begin = (" << begin_cell.x << ';' << begin_cell.y << ')' << endl;
        *log_stream << "Info: " << __FUNCTION__ << "(): end = (" << end_cell.x << ';' << end_cell.y << ')' << endl;
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
    // Unicode symbols for displaying path
    string **cells = new string *[y];
    for (int i = 0; i < y; ++i) {
        cells[i] = new string [x];
    }
    for (int j = 0; j < y; ++j) {
        for (int i = 0; i < x; ++i) {
            cells[j][i] = ' ';
        }
    }
    for (size_t i = 0; i < path.size(); ++i) {
        // if (i > 0) {
        //         }
        //         }
        //     }
        // } else {
            cells[path[i].y][path[i].x] = "\u2022";
        // }
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
                    *main_stream << cells[j][i - 1];
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
    for (int i = 0; i < y; ++i) {
        delete [] cells[i];
    }
    delete [] cells;
    *log_stream << "Info: " << __FUNCTION__ << "(): printing complete" << endl;
    return 0;
}

int Laby::solve() {
    *log_stream << "Info: " << __FUNCTION__ << "(): solving..." << endl;
    if (walls_v == nullptr || walls_h == nullptr) {
        *log_stream << "Warning: " << __FUNCTION__ << "(): cannot solve, walls not initialized" << endl;
        return 1;
    }

    path.push_back(begin_cell);
    int it;
    for (it = 0; it < iter_limit; ++it) {
        // Check whether current cell is exit
        if (path.back() == end_cell) {
            break;
        }
        // Check current cell's walls
        // Northern wall
        if (walls_h[path.back().y][path.back().x]) {
            path.back().sides[CellDir::north] = CellDirStatus::rejected;
        }
        // Eastern wall
        if (walls_v[path.back().y][path.back().x + 1]) {
            path.back().sides[CellDir::east] = CellDirStatus::rejected;
        }
        // Southern wall
        if (walls_h[path.back().y + 1][path.back().x]) {
            path.back().sides[CellDir::south] = CellDirStatus::rejected;
        }
        // Western wall
        if (walls_v[path.back().y][path.back().x]) {
            path.back().sides[CellDir::west] = CellDirStatus::rejected;
        }
        for (int i = 0; i < 4; ++i) {
            *main_stream << path.back().x << ' ' << path.back().y << ' ' << i << ' ' << static_cast<short>(*path.back().array_sides[i]) << endl;
        }
        // Choose next direction to follow
        path.back().active_dir = CellDir::undef;
        for (int i = 0; i < dirs; ++i) {
            if (*path.back().array_sides[i] == CellDirStatus::undef) {
                path.back().active_dir = static_cast<CellDir>(i + 1);
                break;
            }
        }
        CellDir next_dir { path.back().active_dir };
        *main_stream << "next dir: " << static_cast<short>(next_dir) << endl;
        // If there are no one direction remains, fall back to previous cell
        if (next_dir == CellDir::undef) {
            if (path.empty()) {
                *log_stream << "Warning: " << __FUNCTION__ << "(): path cycles, exit solving function" << endl;
                break;
            }
            path.pop_back();
            *main_stream << "dead end, deleting" << endl;
            path.back().sides[path.back().active_dir] = CellDirStatus::rejected;
            continue;
        }
        // Do one step in "next_dir" direction
        path.push_back(path.back());
        *main_stream << "making a copy" << endl;
        *main_stream << "this cell:" << endl;
        for (int i = 0; i < dirs; ++i) {
            *main_stream << path[path.size() - 2].x << ' ' << path[path.size() - 2].y << ' ' << i << ' ' << static_cast<short>(*path[path.size() - 2].array_sides[i]) << endl;
        }
        path.back().sides[opposite_dir(next_dir)] = CellDirStatus::origin;
        switch (next_dir) {
            case CellDir::north:
                --path.back().y;
                break;
            case CellDir::east:
                ++path.back().x;
                break;
            case CellDir::south:
                ++path.back().y;
                break;
            case CellDir::west:
                --path.back().x;
                break;
            default:
                break;
        }
        *main_stream << "new cell:" << endl;
        for (int i = 0; i < dirs; ++i) {
            *main_stream << path.back().x << ' ' << path.back().y << ' ' << i << ' ' << static_cast<short>(*path.back().array_sides[i]) << endl;
        }
        print();
    }
    if (it >= iter_limit) {
        *log_stream << "Warning: " << __FUNCTION__ << "(): path not found in " << it << " iterations" << endl;
    }

    *log_stream << "Info: " << __FUNCTION__ << "(): solving complete" << endl;
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

CellDir Laby::opposite_dir(CellDir dir) {
    switch (dir) {
        case CellDir::north:
            return CellDir::south;
        case CellDir::east:
            return CellDir::west;
        case CellDir::south:
            return CellDir::north;
        case CellDir::west:
            return CellDir::east;
        default:
            return CellDir::undef;
    }
}
