#ifndef _LABY_H_
#define _LABY_H_

#include <ostream>
#include <fstream>
#include <vector>

using namespace std;

struct Cell {
    Cell() {};
    Cell(int x, int y) : x(x), y(y) {};
    Cell(const Cell &c) : x(c.x), y(c.y) {};
    ~Cell() {};
    int x { 0 };
    int y { 0 };
    bool operator==(const Cell &c) {
        return x == c.x && y == c.y;
    }
    bool operator!=(const Cell &c) {
        return x != c.x || y != c.y;
    }
};

class Laby {
public:
    Laby();
    Laby(int x, int y, ofstream *l, ostream *m);
    ~Laby();

    int print(); // Outputs labyrinth

private:
    bool **walls_v       { nullptr }; // vertical walls
    bool **walls_h       { nullptr }; // horizontal walls
    int  x               { 0 };
    int  y               { 0 };
    int iter_limit       { 1000 };
    Cell begin_cell;
    Cell end_cell;
    vector <Cell> path;

    ofstream *log_stream { nullptr };
    ostream *main_stream { nullptr };

    int generate();
    int solve();
    bool rand_bool();
};

#endif // _LABY_H_
