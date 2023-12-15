#include "Wall.hpp"
#include "Headers.hpp"
Wall::Wall()
{
}
void Wall::init(int n = 4)
{
    n_sides = n;
    center_x = 0.0;
    center_y = 0.0;
    radius = sqrt(72);
    vertex = generateCorners();
}
void Wall::draw()
{
    // glDisable(GL_DEPTH_TEST); // je object kache setake rakhe
    glPushMatrix();
    {
        glColor4f(1, 0, 0, 0.7);
        for (int i = 0; i < n_sides; i++)
        {
            glBegin(GL_QUADS);
            glVertex3f(vertex[i].x, vertex[i].y, 0.0);
            glVertex3f(vertex[(i + 1) % n_sides].x, vertex[(i + 1) % n_sides].y, 0.0);
            glVertex3f(vertex[(i + 1) % n_sides].x, vertex[(i + 1) % n_sides].y, 0.8);
            glVertex3f(vertex[i].x, vertex[i].y, 0.8);
            glEnd();
        }
    }

    {
        glColor4f(0, 0, 0, 1);
        for (int i = 0; i < n_sides; i++)
        {
            glBegin(GL_LINE_STRIP);
            glVertex3f(vertex[i].x, vertex[i].y, 0.0);
            glVertex3f(vertex[(i + 1) % n_sides].x, vertex[(i + 1) % n_sides].y, 0.0);
            glVertex3f(vertex[(i + 1) % n_sides].x, vertex[(i + 1) % n_sides].y, 0.8);
            glVertex3f(vertex[i].x, vertex[i].y, 0.8);
            glEnd();
        }
    }
    glPopMatrix();
    // glEnable(GL_DEPTH_TEST); // je object kache setake rakhe
}

vector<Vec2D> Wall::generateCorners()
{
    vector<Vec2D> vertices;
    for (int i = 0; i < n_sides; ++i)
    {
        double angle = radians(45) + radians(360) * i / n_sides;
        double x = center_x + radius * cos(angle);
        double y = center_y + radius * sin(angle);
        vertices.push_back({x, y});
    }
    return vertices;
}

void Wall::increase()
{
    n_sides = min(n_sides + 1, 40);
    vertex = generateCorners();
}
void Wall::decrease()
{
    n_sides = max(3, n_sides - 1);
    vertex = generateCorners();
}