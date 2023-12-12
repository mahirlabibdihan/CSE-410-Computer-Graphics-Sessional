#ifndef _COLLISION_MANAGER_H
#define _COLLISION_MANAGER_H
// #include "Ball.hpp"
#include "Wall.hpp"
#include "Vec3.hpp"
#include "GLM.hpp"
#include <iostream>
using namespace std;

class Ball;
class CollisionManager
{
public:
    Ball *ball;
    Wall *wall;
    int nearest_wall;
    CollisionManager(Ball *ball, Wall *wall);
    void init();
    double getNearestWall(bool back);
    void handleCollision(bool back);
    void predictCollision();
    double getMinDistance(int nearest_wall, bool back);
    bool checkCollision(bool back);
    bool check();
};
#endif