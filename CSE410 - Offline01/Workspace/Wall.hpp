
#ifndef _WALL_H
#define _WALL_H

#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

#include "Vec3.hpp"
#include <vector>
#include <cmath>
#include "GLM.hpp"
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