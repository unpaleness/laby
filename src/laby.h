#ifndef _LABY_H_
#define _LABY_H_

typedef unsigned short int us_int;

class Laby {
public:
    Laby();
    Laby(int x, int y);
    ~Laby();

    int get_x();
    int get_y();
    int get_field(int i, int j);

private:
    us_int *field { nullptr };
    int x         { 0 };
    int y         { 0 };
};

#endif // _LABY_H_
