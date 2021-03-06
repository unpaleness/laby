#include <cstdlib>
#include <set>

#include "laby.h"

using namespace Labyrinth;

/****************\
* PUBLIC METHODS *
\****************/

Laby::Laby() {}

Laby::Laby(
    ofstream *l,
    ostream *m,
    int x,
    int y,
    int it,
    int x1,
    int y1,
    int x2,
    int y2
) : x(x), y(y), iter_limit(it), log_stream(l), main_stream(m) {
    *log_stream << "Info: " << __FUNCTION__ << "(): constructing..." << endl;
    if (x > 0 && y > 0) {
        // begin_cell.set_x(rand() % x);
        // begin_cell.set_y(rand() % y);
        // end_cell.set_x(rand() % x);
        // end_cell.set_y(rand() % y);
        begin_cell.set_x(x1);
        begin_cell.set_y(y1);
        end_cell.set_x(x2);
        end_cell.set_y(y2);
        walls_v = new bool *[y];
        for (int i = 0; i < y; ++i) {
            walls_v[i] = new bool [x + 1];
        }
        walls_h = new bool *[y + 1];
        for (int i = 0; i < y + 1; ++i) {
            walls_h[i] = new bool [x];
        }
        *log_stream << "Info: " << __FUNCTION__ << "(): begin = (" << begin_cell.get_x() << ';' << begin_cell.get_y() << ')' << endl;
        *log_stream << "Info: " << __FUNCTION__ << "(): end = (" << end_cell.get_x() << ';' << end_cell.get_y() << ')' << endl;
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
    char   c_empty { ' ' };
    string s_wall  { "\u2588" };
    string s_path  { "\033[31m\u2588\033[0m" };
    string s_temp;
    // Set up matrix of numeric values of elements
    short **cells = new short *[y * 2 + 1];
    for (int i = 0; i < y * 2 + 1; ++i) {
        cells[i] = new short [x * 2 + 1];
    }
    // Set up empty and node elements
    for (int j = 0; j < y * 2 + 1; ++j) {
        for (int i = 0; i < x * 2 + 1; ++i) {
            if ( j == 0 || i == 0 || j == y * 2 || i == x * 2 || ( !(j % 2) && !(i % 2) ) ) {
                cells[j][i] = 1;
            } else {
                cells[j][i] = 0;
            }
        }
    }
    // Set up path elements
    for (size_t i = 0; i < path.size(); ++i) {
        cells[path[i].get_y() * 2 + 1][path[i].get_x() * 2 + 1] = 2;
        if (i > 0) {
            cells[path[i - 1].get_y() + path[i].get_y() + 1][path[i - 1].get_x() + path[i].get_x() + 1] = 2;
        }
    }
    // Set up walls' elements
    for (int j = 0; j < y + 1; ++j) {
        for (int i = 0; i < x; ++i) {
            if (walls_h[j][i]) {
                cells[j * 2][i * 2 + 1] = 1;
            }
        }
        if (j < y) {
            for (int i = 0; i < x + 1; ++i) {
                if (walls_v[j][i]) {
                    cells[j * 2 + 1][i * 2] = 1;
                }
            }
        }
    }
    // Output
    for (int j = 0; j < y * 2 + 1; ++j) {
        s_temp.clear();
        for (int i = 0; i < x * 2 + 1; ++i) {
            switch (cells[j][i]) {
                case 2:
                    s_temp += s_path;
                    break;
                case 1:
                    s_temp += s_wall;
                    break;
                default:
                    s_temp += c_empty;
                    break;
            }
        }
        *main_stream << s_temp << endl;
    }
    for (int i = 0; i < y * 2 + 1; ++i) {
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
        if (walls_h[path.back().get_y()][path.back().get_x()]) {
            path.back().set_sides(CellDir::north, CellDirStatus::rejected);
        }
        // Eastern wall
        if (walls_v[path.back().get_y()][path.back().get_x() + 1]) {
            path.back().set_sides(CellDir::east, CellDirStatus::rejected);
        }
        // Southern wall
        if (walls_h[path.back().get_y() + 1][path.back().get_x()]) {
            path.back().set_sides(CellDir::south, CellDirStatus::rejected);
        }
        // Western wall
        if (walls_v[path.back().get_y()][path.back().get_x()]) {
            path.back().set_sides(CellDir::west, CellDirStatus::rejected);
        }
        // Choose next direction to follow
        path.back().set_active_dir(CellDir::undef);
        for (int i = 0; i < dirs; ++i) {
            if (path.back().get_array_sides(i) == CellDirStatus::undef) {
                path.back().set_active_dir(static_cast<CellDir>(i + 1));
                break;
            }
        }
        CellDir next_dir { path.back().get_active_dir() };
        // If there are no one direction remains, fall back to previous cell
        if (next_dir == CellDir::undef) {
            if (path.empty()) {
                *log_stream << "Warning: " << __FUNCTION__ << "(): path cycles, exit solving function" << endl;
                break;
            }
            path.pop_back();
            path.back().set_sides(path.back().get_active_dir(), CellDirStatus::rejected);
            continue;
        }
        // Do one step in "next_dir" direction
        path.push_back(path.back());
        path.back().reset_sides();
        path.back().set_sides(opposite_dir(next_dir), CellDirStatus::origin);
        switch (next_dir) {
            case CellDir::north:
                path.back().dec_y();
                break;
            case CellDir::east:
                path.back().inc_x();
                break;
            case CellDir::south:
                path.back().inc_y();
                break;
            case CellDir::west:
                path.back().dec_x();
                break;
            default:
                break;
        }
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
