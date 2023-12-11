
#ifdef __linux__
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

#include "Point.hpp"
#include "GLM.hpp"
#include <cmath>
using namespace std;

class Ball
{
public:
    PT points[100][100], lower_points[100][100];
    GLfloat radius;
    PT look, right, up, pos;

public:
    void init(double radius, int slices, int stacks)
    {
        this->radius = radius;
        look = PT(1 / sqrt(2), 1 / sqrt(2), 0);
        right = PT(1 / sqrt(2), -1 / sqrt(2), 0);
        up = PT(0, 0, 1);
        pos = PT(0, 0, radius);
        int i, j;
        double h, r;
        for (i = 0; i <= stacks; i++)
        {
            h = radius * sin(((double)i / (double)stacks) * (M_PI / 2));
            r = radius * cos(((double)i / (double)stacks) * (M_PI / 2));
            for (j = 0; j <= slices; j++)
            {
                lower_points[i][j].x = points[i][j].x = r * cos(((double)j / (double)slices) * 2 * M_PI);
                lower_points[i][j].y = points[i][j].y = r * sin(((double)j / (double)slices) * 2 * M_PI);
                points[i][j].z = h;
                lower_points[i][j].z = -h;
            }
        }
    }
    void rotate(double ang, int slices, int stacks)
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

    void draw(double radius, int slices, int stacks)
    {
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
    }
};
