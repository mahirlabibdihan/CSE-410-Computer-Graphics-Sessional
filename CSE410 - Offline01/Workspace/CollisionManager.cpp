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

double CollisionManager::getNearestWall(bool back)
{
    double dist = INT64_MAX;
    int idx = -1;
    for (int i = 0; i < wall->n_sides; i++)
    {
        Vec3 tmp_look = ball->look * (back ? -1 : 1);
        Vec3 A = wall->vertex[i];
        Vec3 B = wall->vertex[(i + 1) % wall->n_sides];
        Vec3 C = ball->pos;
        Vec3 D = ball->pos + ball->look;
        Vec3 dir = B - A;
        double theta = acos(dot2D(dir, tmp_look) / (det2D(dir) * det2D(tmp_look)));

        Vec3 I = findIntersectionInf(A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);
        if (isnanf(I.x) || isnanf(I.y))
        {
            continue;
        }

        Vec3 tmp = I - C;

        double check_theta = acos(min(max(dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)), -1.0), 1.0));
        if (floor(check_theta) == 0 || back)
        {
            double distance = distancePointToPoint(C, I);
            double minus = ball->radius / sin(theta);
            if (dist > distance - minus)
            {
                idx = i;
                dist = distance - minus;
            }
        }
    }
    return idx;
}

void CollisionManager::handleCollision(bool back)
{

    int i = getNearestWall(back);
    // for (int i = 0; i < n; i++)
    {
        Vec3 A = wall->vertex[i];
        Vec3 B = wall->vertex[(i + 1) % wall->n_sides];
        Vec3 C = ball->pos;
        // Vec3 D = ball->pos + (ball->look * 100);
        // if (intersect(A, B, C, D))
        {
            Vec3 direction_vector = A - B;
            Vec3 orthogonal_direction_vector = Vec3(-direction_vector.y, direction_vector.x, 0);
            orthogonal_direction_vector = orthogonal_direction_vector / sqrt(dot(orthogonal_direction_vector, orthogonal_direction_vector));
            ball->look = ball->look - orthogonal_direction_vector * 2 * dot(ball->look, orthogonal_direction_vector);
            ball->right = Vec3(ball->look.y, -ball->look.x, 0); // Orthogonal
        }
    }
    // printf("==============================Collision=================================\n");
}

void CollisionManager::predictCollision()
{
    // printf("==============================Collision=================================\n");
    // int n = 4;
    int i = nearest_wall;
    // for (int i = 0; i < n; i++)
    {
        Vec3 A = wall->vertex[i];
        Vec3 B = wall->vertex[(i + 1) % wall->n_sides];
        Vec3 C = ball->pos;
        Vec3 D = ball->pos + (ball->look * 100);
        // if (intersect(A, B, C, D))
        {
            Vec3 direction_vector = A - B;
            Vec3 orthogonal_direction_vector = Vec3(-direction_vector.y, direction_vector.x, 0);
            orthogonal_direction_vector = orthogonal_direction_vector / sqrt(dot(orthogonal_direction_vector, orthogonal_direction_vector));
            ball->look = ball->look - orthogonal_direction_vector * 2 * dot(ball->look, orthogonal_direction_vector);
            ball->right = Vec3(ball->look.y, -ball->look.x, 0); // Orthogonal
            // break;
        }
    }
}

double CollisionManager::getMinDistance(int nearest_wall, bool back)
{
    double dist = INT64_MAX;
    int i = nearest_wall;
    // for (int i = 0; i < n; i++)
    {
        Vec3 tmp_look = ball->look * (back ? -1 : 1);
        Vec3 A = wall->vertex[i];
        Vec3 B = wall->vertex[(i + 1) % wall->n_sides];
        Vec3 C = ball->pos;
        Vec3 D = ball->pos + (tmp_look * 100);
        // if (intersect(A, B, C, D))
        {
            Vec3 dir = B - A;
            double theta = acos(dot2D(dir, tmp_look) / (det2D(dir) * det2D(tmp_look)));

            // printf("Wall: %d, Theta: %lf\n", i, theta * 180 / PI);

            Vec3 I = findIntersectionInf(A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);

            // if (isnanf(I.x) || isnanf(I.y))
            // {
            //     continue;
            // }

            Vec3 tmp = I - C;

            double check_theta = acos(min(max(dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)), -1.0), 1.0));

            if (floor(check_theta) == 0)
            {
                double distance = distancePointToPoint(C, I);
                double minus = ball->radius / sin(theta);
                printf("%lf\n", ball->radius);
                dist = min(dist, distance - minus);

                printf("Dist: %lf, fULL: %lf, N: %lf, D: %lf, %lf\n", distance - minus, distance, dot2D(tmp, tmp_look), det2D(tmp) * det2D(tmp_look), dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)));
            }
        }
    }
    return dist;
}

bool CollisionManager::checkCollision(bool back)
{
    double min_dist = getMinDistance(getNearestWall(back), back);
    printf("Min_Dist: %lf\n", min_dist);
    if (min_dist <= 1e-9)
    {
        printf("Collusion\n");
        return true;
    }
    return false;
}

bool CollisionManager::check()
{
    if (nearest_wall == -1)
    {
        nearest_wall = getNearestWall(false);
    }
    double min_dist = getMinDistance(nearest_wall, false);
    if (min_dist < 1e-9)
    {
        predictCollision();
        nearest_wall = getNearestWall(false);

        return true;
    }
    return false;
}