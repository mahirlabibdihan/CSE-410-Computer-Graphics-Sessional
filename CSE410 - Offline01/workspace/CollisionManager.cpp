#include "CollisionManager.hpp"
#include "Headers.hpp"

CollisionManager::CollisionManager(Ball *ball, Wall *wall) : ball(ball), wall(wall)
{
    init();
}
void CollisionManager::init()
{
    nearest_wall = -1;
}
void CollisionManager::setNearestWall()
{
    nearest_wall = getNearestWall();
}

double CollisionManager::ballToWallDistance(Vec2D ball_p, Vec2D ball_d, Vec2D wall_p, Vec2D wall_d)
{
    Vec2D I = getIntersectionPoint(wall_p, wall_d, ball_p, ball_d);
    double distance = distancePointToPoint2D(ball_p, I);
    double theta = ball_d.angle(wall_d);
    double minus = ball->radius / sin(theta);
    return distance - minus;
}

double CollisionManager::getNearestWall()
{
    double min_dist = INT64_MAX;
    int idx = -1;
    for (int i = 0; i < wall->n_sides; i++)
    {
        Vec2D ball_look = ball->look * (ball->isBackward() ? -1 : 1);
        Vec2D A = wall->vertex[i];
        Vec2D B = wall->vertex[(i + 1) % wall->n_sides];
        Vec2D C = ball->pos;
        Vec2D wall_dir = B - A;

        if (!doRayIntersectLine(C, ball_look, A, wall_dir))
        {
            continue;
        }

        double dist = ballToWallDistance(C, ball_look, A, wall_dir);
        if (min_dist > dist)
        {
            idx = i;
            min_dist = dist;
        }
    }
    return idx;
}

double CollisionManager::getMinDistance()
{
    Vec2D ball_look = ball->look * (ball->isBackward() ? -1 : 1);
    Vec2D A = wall->vertex[nearest_wall];
    Vec2D B = wall->vertex[(nearest_wall + 1) % wall->n_sides];
    Vec2D C = ball->pos;
    Vec2D wall_dir = B - A;

    return ballToWallDistance(C, ball_look, A, wall_dir);
}

void CollisionManager::handleCollision()
{
    Vec2D ball_look = ball->look * (ball->isBackward() ? -1 : 1);
    int i = nearest_wall;
    Vec2D A = wall->vertex[i];
    Vec2D B = wall->vertex[(i + 1) % wall->n_sides];
    Vec2D wall_dir = B - A;
    ball_look = ball_look.reflect(wall_dir);
    ball->look = ball_look * (ball->isBackward() ? -1 : 1);
    ball->right = -ball->look.perp(); // Clockwise Orthogonal
    this->setNearestWall();
}

bool CollisionManager::checkCollision()
{
    double min_dist = getMinDistance();
    if (min_dist <= 1e-9)
    {
        return true;
    }
    return false;
}

bool CollisionManager::check()
{
    if (nearest_wall == -1)
    {
        nearest_wall = getNearestWall();
    }
    double min_dist = getMinDistance();
    if (min_dist < 1e-9)
    {
        handleCollision();
        return true;
    }
    return false;
}