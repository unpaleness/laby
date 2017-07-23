#ifndef _LABY_H_
#define _LABY_H_

#include <ostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

enum class CellDir : short {
    undef = 0,
    north = 1,
    east  = 2,
    south = 3,
    west  = 4
};

enum class CellDirStatus : short {
    undef    = 0, // Not tested
    origin   = 1, // The path comes from this direction
    rejected = 2  // Testes and not a valid direction for path
};

class Cell {
public:
    Cell() {};
    Cell(int x, int y) : x(x), y(y) { init(); };
    Cell(const Cell &c) : x(c.x), y(c.y) { init(); };
    ~Cell() {};
    bool operator==(const Cell &c) {
        return x == c.x && y == c.y;
    }
    bool operator!=(const Cell &c) {
        return x != c.x || y != c.y;
    }

    int x { 0 };
    int y { 0 };
    CellDir active_dir { CellDir::undef };
    map <CellDir, CellDirStatus> sides;
    CellDirStatus *array_sides[4];

private:
    void init() {
        sides[CellDir::north] = CellDirStatus::undef;
        sides[CellDir::east]  = CellDirStatus::undef;
        sides[CellDir::south] = CellDirStatus::undef;
        sides[CellDir::west]  = CellDirStatus::undef;
        array_sides[0] = &sides[CellDir::north];
        array_sides[1] = &sides[CellDir::east];
        array_sides[2] = &sides[CellDir::south];
        array_sides[3] = &sides[CellDir::west];
    }
};

class Laby {
public:
    Laby();
    Laby(int x, int y, ofstream *l, ostream *m);
    ~Laby();

    int print(); // Outputs labyrinth
    int solve(); // Finds the path

private:
    bool **walls_v      { nullptr }; // vertical walls
    bool **walls_h      { nullptr }; // horizontal walls
    int  x              { 0 };
    int  y              { 0 };
    int iter_limit      { 20 };
    int dirs            { 4 };
    Cell begin_cell;
    Cell end_cell;
    vector <Cell> path;

    ofstream *log_stream { nullptr };
    ostream *main_stream { nullptr };

    int generate();
    bool rand_bool();
    CellDir opposite_dir(CellDir dir);
};

#endif // _LABY_H_
