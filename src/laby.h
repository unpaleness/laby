#ifndef _LABY_H_
#define _LABY_H_

#include <ostream>
#include <fstream>

using namespace std;

typedef unsigned short int us_int;

class Laby {
public:
    Laby();
    Laby(int x, int y, ofstream *l, ostream *m);
    ~Laby();

    int print(); // Outputs labyrinth

private:
    bool *walls_v     { nullptr }; // vertical walls
    bool *walls_h     { nullptr }; // horizontal walls
    int  x            { 0 };
    int  y            { 0 };

    ofstream *log_stream;
    ostream *main_stream;

    int generate();
};

#endif // _LABY_H_
