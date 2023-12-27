#include "Headers.hpp"

Camera *camera = new Camera();

class MagicCube
{
    double maxTriangleCoord = 1.6;
    double triangleCoord = 1.6;
    double angle = 45.0;

public:
    void drawTriangle()
    {
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(1, 0, 0);
            glVertex3f(0, 1, 0);
            glVertex3f(0, 0, 1);
        }
        glEnd();
    }

    void drawFaces()
    {
        for (int j = 0; j < 2; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                glPushMatrix();
                glColor3f((i + 1 - j) % 2, (i + j) % 2, 1.0f);
                glRotatef(90 * i, 0, 0, 1);
                glRotatef((j - 1) * 180, 1, 1, 0);
                glTranslatef(maxTriangleCoord / 3.0, maxTriangleCoord / 3.0, maxTriangleCoord / 3.0);
                glScalef(triangleCoord, triangleCoord, triangleCoord);
                glTranslatef(-1 / 3.0, -1 / 3.0, -1 / 3.0);
                drawTriangle();
                glPopMatrix();
            }
        }
    }

    void drawCylinderSegment(double radius, double height, int n_segments = 100)
    {
        vector<Vec2D> vertices;

        double arcAngle = radians(70.5287794);

        double x, y;
        for (int i = 0; i <= n_segments; i++)
        {
            double radian = (0 - arcAngle) / 2 + arcAngle * (1.0 * i / n_segments);
            x = radius * cos(radian);
            y = radius * sin(radian);
            vertices.push_back({x, y});
        }

        glBegin(GL_QUADS);
        for (int i = 0; i < n_segments; i++)
        {
            glVertex3f(vertices[i].x, vertices[i].y, height / 2);
            glVertex3f(vertices[i + 1].x, vertices[i + 1].y, height / 2);
            glVertex3f(vertices[i + 1].x, vertices[i + 1].y, -height / 2);
            glVertex3f(vertices[i].x, vertices[i].y, -height / 2);
        }
        glEnd();
    }

    void drawEdges()
    {
        glColor3f(1.0f, 1.0f, 0.0f);
        double diff = maxTriangleCoord - triangleCoord;
        double radius = (diff / sqrt(2)) * cos(radians(70.5287794 / 2));
        // Each side of triangle has length = triangleCoord * sqrt(2)
        double height = triangleCoord * sqrt(2);
        double distToCenter = triangleCoord / sqrt(2);

        for (int j = 0; j < 3; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                glPushMatrix();
                glRotatef(90, j == 0, j == 1, j == 2);
                glRotatef((45 + i * 90), 0, 1, 0);
                glTranslatef(distToCenter, 0, 0);
                drawCylinderSegment(radius, height);
                glPopMatrix();
            }
        }
    }

    void drawSphereSegment(double radius, int n_segments)
    {
        vector<vector<Vec3D>> vertices;

        double x, y, z;
        for (int i = 0; i <= n_segments; i++)
        {
            vertices.push_back(vector<Vec3D>());
            for (int j = 0; j <= n_segments; j++)
            {
                x = -1 + 2.0 * i / n_segments;
                y = -1 + 2.0 * j / n_segments;
                vertices[i].push_back({x, y, 1});
                vertices[i][j] = vertices[i][j].normalize() * radius;
            }
        }

        for (int i = 0; i < n_segments; i++)
        {
            for (int j = 0; j < n_segments; j++)
            {
                glBegin(GL_QUADS);
                {
                    glVertex3f(vertices[i][j].x, vertices[i][j].y, vertices[i][j].z);
                    glVertex3f(vertices[i][j + 1].x, vertices[i][j + 1].y, vertices[i][j + 1].z);
                    glVertex3f(vertices[i + 1][j + 1].x, vertices[i + 1][j + 1].y, vertices[i + 1][j + 1].z);
                    glVertex3f(vertices[i + 1][j].x, vertices[i + 1][j].y, vertices[i + 1][j].z);
                }
                glEnd();
            }
        }
    }

    void drawVertices()
    {
        double diff = maxTriangleCoord - triangleCoord;
        double radius = (diff / sqrt(2)) * cos(radians(70.5287794 / 2));

        for (int j = 0; j < 3; j++)
        {
            for (int i = 0; i < 2; i++)
            {
                glPushMatrix();
                {
                    glColor3f(j == 2, j == 1, j == 0);
                    glRotatef(90, j == 0, j == 1, j == 2);
                    glRotatef(180 * i, 0, 1, 0);
                    glTranslatef(0, 0, triangleCoord);
                    drawSphereSegment(radius, 10);
                }
                glPopMatrix();
            }
        }
    }
    void draw()
    {
        glPushMatrix();
        glRotatef(angle, 0, 0, 1);
        drawFaces();
        drawEdges();
        drawVertices();
        glPopMatrix();
    }
    void rotateClock()
    {
        angle -= 5.0f;
    }
    void rotateCounterClock()
    {
        angle += 5.0f;
    }
    void sphereToOcta()
    {
        triangleCoord += 0.1;
        if (triangleCoord > maxTriangleCoord)
        {
            triangleCoord = maxTriangleCoord;
        }
    }
    void octaToSphere()
    {
        triangleCoord -= 0.1;
        if (triangleCoord < 0)
        {
            triangleCoord = 0;
        }
    }
};

MagicCube mc;
void DGraphics::display()
{
    camera->set();
    // Clear all pixels
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mc.draw();
    // camera->drawReference();

    // glPushMatrix();
    // glScalef(5, 5, 5);
    // DGraphics::drawAxis();
    // glPopMatrix();
}

void DGraphics::init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Vec3D pos = Vec3D(5.5, 0, 4);
    Vec3D right = Vec3D(0, 1, 0);
    Vec3D up = Vec3D(-1 / sqrt(2), 0, 1 / sqrt(2)).rotate(right, radians(15));
    Vec3D look = Vec3D(-1 / sqrt(2), 0, -1 / sqrt(2)).rotate(right, radians(15));
    camera->init(pos, up, right, look);
}

void DGraphics::keyboard(unsigned char key)
{
    camera->keyPress(key);
    switch (key)
    {
    case 'd':
        mc.rotateCounterClock();
        break;
    case 'a':
        mc.rotateClock();
        break;
    case ',':
        mc.octaToSphere();
        break;
    case '.':
        mc.sphereToOcta();
        break;
    case 27:
        glutLeaveMainLoop();
        break;
    default:
        break;
    }
}
void DGraphics::specialKeyboard(int key)
{
    camera->specialKeyPress(key);
}

void DGraphics::mouseScroll(int dir)
{
    camera->mouseScroll(dir);
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv); // Initialize GLUT // Without this : Other glut functions can't be called .
    DGraphics::initialize(640, 640, "Magic Cube");
    glutMainLoop(); // Enter the event-processing loop
    return 0;
}