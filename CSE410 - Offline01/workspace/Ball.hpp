#ifndef _BALL_H
#define _BALL_H

#include "CollisionManager.hpp"
#include "Ball.hpp"
#include "Vec3D.hpp"
#include "Vec2D.hpp"

class CollisionManager;
class Ball
{
public:
    Vec3D points[100][100], lower_points[100][100];
    Vec2D look, right, pos;
    Vec3D up;
    bool manual, backward;
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
    bool isManualControl();
    void switchControl();
    void switchDirection();
    bool isBackward();
};
#endif