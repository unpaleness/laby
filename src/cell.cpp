#include "cell.h"

using namespace Labyrinth;

/****************\
* PUBLIC METHODS *
\****************/

Cell::Cell() {
    init();
}

Cell::Cell(int x, int y) : x(x), y(y) {
    init();
}

Cell::Cell(const Cell &c) : x(c.get_x()), y(c.get_y()) {
    init();
    sides[CellDir::north] = c.get_sides(CellDir::north);
    sides[CellDir::east]  = c.get_sides(CellDir::east);
    sides[CellDir::south] = c.get_sides(CellDir::south);
    sides[CellDir::west]  = c.get_sides(CellDir::west);
}

bool Cell::operator==(const Cell &c) {
    return x == c.x && y == c.y;
}

bool Cell::operator!=(const Cell &c) {
    return x != c.x || y != c.y;
}

int Cell::get_x() const { return x; }
int Cell::get_y() const { return y; }
CellDir       Cell::get_active_dir()       const { return active_dir; }
CellDirStatus Cell::get_sides(CellDir cd)  const { return sides.at(cd); }
CellDirStatus Cell::get_array_sides(int i) const { return *array_sides[i]; }
void Cell::set_x(int x) { this->x = x; }
void Cell::set_y(int y) { this->y = y; }
void Cell::set_active_dir(CellDir cd)                { active_dir = cd; }
void Cell::set_sides(CellDir cd, CellDirStatus cds)  { sides[cd] = cds; }
void Cell::set_array_sides(int i, CellDirStatus cds) { *array_sides[i] = cds; }

int Cell::inc_x() { return ++x; }
int Cell::inc_y() { return ++y; }
int Cell::dec_x() { return --x; }
int Cell::dec_y() { return --y; }
void Cell::reset_sides() {
    sides[CellDir::north] = CellDirStatus::undef;
    sides[CellDir::east]  = CellDirStatus::undef;
    sides[CellDir::south] = CellDirStatus::undef;
    sides[CellDir::west]  = CellDirStatus::undef;
}

/*****************\
* PRIVATE METHODS *
\*****************/

void Cell::init() {
    reset_sides();
    array_sides[0] = &sides[CellDir::north];
    array_sides[1] = &sides[CellDir::east];
    array_sides[2] = &sides[CellDir::south];
    array_sides[3] = &sides[CellDir::west];
}
