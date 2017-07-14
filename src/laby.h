#ifndef _LABY_H_
#define _LABY_H_

#include <ostream>
#include <fstream>
#include <vector>

using namespace std;

// struct Cell {
//     int x { 0 };
//     int y { 0 };
// };

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
    // Cell begin           { 0, 0 };
    // Cell end             { 0, 0 };

    ofstream *log_stream { nullptr };
    ostream *main_stream { nullptr };

    int generate();
    int solve();
    bool rand_bool();
};

#endif // _LABY_H_
