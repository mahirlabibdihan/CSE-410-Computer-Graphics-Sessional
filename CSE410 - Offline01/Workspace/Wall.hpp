
#ifndef _WALL_H
#define _WALL_H

#include "Vec3.hpp"
#include <vector>
using namespace std;

class Wall
{
public:
    int n_sides;
    double center_x, center_y, radius;
    vector<Vec3> vertex;

public:
    Wall();
    void init(int n);
    void draw();
    vector<Vec3> generateCorners();
    void increase();
    void decrease();
};
#endif