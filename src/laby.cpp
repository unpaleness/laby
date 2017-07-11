#include <iostream>

#include "laby.h"

/****************\
* PUBLIC METHODS *
\****************/

Laby::Laby() {}

Laby::Laby(int x, int y) : x(x), y(y) {
    field = new us_int [y * x];
    for (int i = 0; i < y * x; ++i) {
        field[i] = i;
    }
}

Laby::~Laby() {
    if (field != nullptr) {
        std::cout << "I'm cleaning memory" << std::endl;
        delete [] field;
    }
}

int Laby::get_x() {
    return x;
}

int Laby::get_y() {
    return y;
}

int Laby::get_field(int i, int j) {
    return field[j * x + i];
}
