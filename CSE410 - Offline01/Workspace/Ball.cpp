#include "Ball.hpp"

Ball::Ball(double radius, int slices, int stacks)
{
    this->radius = radius;
    this->slices = slices;
    this->stacks = stacks;
}
void Ball::init()
{
    look = Vec3(1 / sqrt(2), 1 / sqrt(2), 0);
    right = Vec3(1 / sqrt(2), -1 / sqrt(2), 0);
    up = Vec3(0, 0, 1);
    pos = Vec3(0, 0, radius);

    for (int i = 0; i <= stacks; i++)
    {
        double h = radius * sin(((double)i / (double)stacks) * (M_PI / 2));
        double r = radius * cos(((double)i / (double)stacks) * (M_PI / 2));
        for (int j = 0; j <= slices; j++)
        {
            lower_points[i][j].x = points[i][j].x = r * cos(((double)j / (double)slices) * 2 * M_PI);
            lower_points[i][j].y = points[i][j].y = r * sin(((double)j / (double)slices) * 2 * M_PI);
            points[i][j].z = h;
            lower_points[i][j].z = -h;
        }
    }
}

void Ball::rotate(double ang)
{
    int i, j;
    for (i = 0; i <= stacks; i++)
    {
        for (j = 0; j <= slices; j++)
        {
            rotate3D(points[i][j], right, radians(ang));
            rotate3D(lower_points[i][j], right, radians(ang));
        }
    }
}

void Ball::draw()
{
    glPushMatrix();
    // glTranslatef(x, y, 0);
    // glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);

    int i, j;
    double h, r;
    // generate points

    // draw quads using generated points
    for (i = 0; i < stacks; i++)
    {
        for (j = 0; j < slices; j++)
        {
            glBegin(GL_QUADS);
            {
                // upper hemisphere
                glColor3f(j % 2, (j + 1) % 2, 0);
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);

                // lower hemisphere
                glColor3f((j + 1) % 2, j % 2, 0);
                glVertex3f(lower_points[i][j].x, lower_points[i][j].y, lower_points[i][j].z);
                glVertex3f(lower_points[i][j + 1].x, lower_points[i][j + 1].y, lower_points[i][j + 1].z);
                glVertex3f(lower_points[i + 1][j + 1].x, lower_points[i + 1][j + 1].y, lower_points[i + 1][j + 1].z);
                glVertex3f(lower_points[i + 1][j].x, lower_points[i + 1][j].y, lower_points[i + 1][j].z);
            }
            glEnd();
        }
    }
    // glPopMatrix();
    glPopMatrix();

    // Direction arrow
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(fmod((atan2(look.y, look.x) * 180 / M_PI) + 360.0, 360.0), up.x, up.y, up.z);
    // glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glColor3f(0.0, 0.0, 1.0); // Red color for the cylinder
    DGraphics::drawArrow(1.0);
    // glPopMatrix();
    glPopMatrix();
}

void Ball::moveForward(CollisionManager *collision_manager)
{
    int nearest_wall = collision_manager->getNearestWall(false);
    printf("%d\n", nearest_wall);
    pos = pos + (look * min(0.2, collision_manager->getMinDistance(nearest_wall, false)));
    rotate(-20);
    if (collision_manager->checkCollision(false))
    {
        collision_manager->handleCollision(false);
    }
    printf("Pass");
}
void Ball::moveBackward(CollisionManager *collision_manager)
{
    pos = pos - (look * min(0.2, collision_manager->getMinDistance(collision_manager->getNearestWall(true), true)));
    rotate(20);
    if (collision_manager->checkCollision(true))
    {
        collision_manager->handleCollision(true);
    }
}

void Ball::rotateClock()
{
    rotate3D(right, up, -radians(15));
    rotate3D(look, up, -radians(15));
}
void Ball::rotateCounterClock()
{
    rotate3D(right, up, radians(15));
    rotate3D(look, up, radians(15));
}