
#ifdef __linux__
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

void drawCheckerboard()
{
    int size = 100;
    for (int i = -size; i < size; i += 2)
    {
        for (int j = -size; j < size; j += 2)
        {
            if ((i + j) / 2 % 2 == 0)
            {
                glColor3f(1.0, 1.0, 1.0); // White
            }
            else
            {
                glColor3f(0.0, 0.0, 0.0); // Black
            }

            glBegin(GL_QUADS);
            glVertex2f(i, j);
            glVertex2f(i + 2, j);
            glVertex2f(i + 2, j + 2);
            glVertex2f(i, j + 2);
            glEnd();
        }
    }
}

void drawSquare(double a)
{
    glBegin(GL_QUADS);
    {
        // Anticlock
        glColor4f(1, 0, 0, 1);
        glVertex2f(a, a);
        glColor4f(0, 1, 0, 1);
        glVertex2f(a, -a);
        glColor4f(0, 0, 1, 1);
        glVertex2f(-a, -a);
        glColor4f(1, 0, 1, 1);
        glVertex2f(-a, a);
    }
    glEnd();
    glBegin(GL_LINES);
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(a, a);
        glVertex2f(a, -a);
        glVertex2f(-a, -a);
        glVertex2f(-a, a);
    }
    glEnd();
}

void drawAxis()
{
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(5, 0, 0);
        glVertex3f(-5, 0, 0);
        glVertex3f(0.0f, 5, 0);
        glVertex3f(0.0f, -5, 0);
        glVertex3f(0.0f, 0, 5);
        glVertex3f(0.0f, 0, -5);
    }
    glEnd();
}
void drawCube()
{
    // Front face
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0); // Red
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glEnd();

    // Back face
    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 0.0); // Green
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glEnd();

    // Connect front and back faces with lines
    glColor3f(0.0, 0.0, 0.0); // Black
    glBegin(GL_LINES);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, -1.0, -1.0);

    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);

    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0);

    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glEnd();
}
const int numSegments = 50;
void drawCylinder(float radius, float height)
{
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= numSegments; ++i)
    {
        float theta = 2.0 * M_aaaaaaaaaaaaaaaaaaaaaPI * static_cast<float>(i) / numSegments;
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        glVertex3f(x, y, 0.0);
        glVertex3f(x, y, height);
    }
    glEnd();
}

void drawCone(float radius, float height)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0, 0.0, height); // Apex
    for (int i = 0; i <= numSegments; ++i)
    {
        float theta = 2.0 * M_PI * static_cast<float>(i) / numSegments;
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        glVertex3f(x, y, 0.0);
    }
    glEnd();
}
void drawArrow(double length)
{
    drawCylinder(0.05, length);
    glTranslatef(0.0, 0.0, length); // Move the cone up to connect with the cylinder
    drawCone(0.1, 0.5);
}
