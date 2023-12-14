
#ifndef _WALL_H
#define _WALL_H

#include "Vec2D.hpp"
#include <vector>
using namespace std;

class Wall
{
public:
    int n_sides;
    double center_x, center_y, radius;
    vector<Vec2D> vertex;

public:
    Wall();
    void init(int n);
    void draw();
    vector<Vec2D> generateCorners();
    void increase();
    void decrease();
};
#endif