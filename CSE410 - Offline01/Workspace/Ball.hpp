#ifndef _BALL_H
#define _BALL_H

#include "CollisionManager.hpp"
#include "Ball.hpp"
#include "Vec3.hpp"

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