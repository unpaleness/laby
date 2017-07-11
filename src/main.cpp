#include <iostream>

#include "laby.h"

int main() {
    Laby laby(10, 10);
    for (int j = 0; j < laby.get_y(); ++j) {
        for (int i = 0; i < laby.get_x(); ++i)
            std::cout << ' ' << laby.get_field(i, j);
        std::cout << std::endl;
    }
    return 0;
}
