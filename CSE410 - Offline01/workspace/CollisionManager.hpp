#ifndef _COLLISION_MANAGER_H
#define _COLLISION_MANAGER_H
#include "Ball.hpp"
#include "Wall.hpp"
class Ball;
class CollisionManager
{
public:
    Ball *ball;
    Wall *wall;
    int nearest_wall;
    CollisionManager(Ball *ball, Wall *wall);
    void init();
    double getNearestWall();
    void setNearestWall();
    void handleCollision();
    double ballToWallDistance(Vec2D ball_p, Vec2D ball_d, Vec2D wall_p, Vec2D wall_d);
    double getMinDistance();
    bool checkCollision();
    bool check();
};
#endif