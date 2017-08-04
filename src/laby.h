#ifndef _LABY_H_
#define _LABY_H_

#include <ostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

#include "cell.h"

namespace Labyrinth {

class Laby {
public:
    Laby();
    Laby(ofstream *l, ostream *m, int x, int y, int it, int x1, int y1, int x2, int y2);
    ~Laby();

    int print(); // Outputs labyrinth
    int solve(); // Finds the path

private:
    bool **walls_v      { nullptr }; // vertical walls
    bool **walls_h      { nullptr }; // horizontal walls
    int x               { 0 };
    int y               { 0 };
    int iter_limit      { 0 };
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

}

#endif // _LABY_H_
