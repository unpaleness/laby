#ifndef _CELL_H_
#define _CELL_H_

#include <map>

using namespace std;

namespace Labyrinth {

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
    Cell();
    Cell(int x, int y);
    Cell(const Cell &c);
    ~Cell() {};

    bool operator==(const Cell &c);
    bool operator!=(const Cell &c);

    int get_x() const;
    int get_y() const;
    CellDir       get_active_dir() const;
    CellDirStatus get_sides(CellDir cd) const;
    CellDirStatus get_array_sides(int i) const;
    void set_x(int x);
    void set_y(int y);
    void set_active_dir(CellDir cd);
    void set_sides(CellDir cd, CellDirStatus cds);
    void set_array_sides(int i, CellDirStatus cds);

    int inc_x();
    int inc_y();
    int dec_x();
    int dec_y();
    void reset_sides();

private:
    int x { 0 };
    int y { 0 };
    CellDir active_dir { CellDir::undef };
    map <CellDir, CellDirStatus> sides;
    CellDirStatus *array_sides[4];

    void init();
};

}

#endif // _CELL_H_
