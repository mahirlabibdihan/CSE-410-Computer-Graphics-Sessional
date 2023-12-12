#ifndef _BALL_H
#define _BALL_H

#ifdef __linux__
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

#include "Vec3.hpp"
#include "GLM.hpp"
#include <cmath>
// #include "CollisionManager.hpp"
using namespace std;

class CollisionManager;
class Ball
{
public:
    Vec3 points[100][100], lower_points[100][100];
    Vec3 look, right, up, pos;
    bool animate;
    double radius, slices, stacks;

public:
    Ball(double radius, int slices, int stacks);
    void init();
    void rotate(double ang);
    void draw();
    void moveForward(CollisionManager *collision_manager);
    void moveBackward(CollisionManager *collision_manager);
    void rotateClock();
    void rotateCounterClock();
};
#endif