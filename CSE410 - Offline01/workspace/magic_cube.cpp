#include "Headers.hpp"

Camera *camera = new Camera();

double maxTriangleVertex = 1.6;
double triangleVertex = 1.6;

void drawTriangle()
{
    // glColor3f(1.0,0.0,0.0);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(1, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(0, 0, 1);
    }
    glEnd();
}

void drawPyramids()
{
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glColor3f((i + 1) % 2, i % 2, 1.0f);
        glRotatef(90 * i, 0, 0, 1);
        glTranslatef(maxTriangleVertex / 3.0, maxTriangleVertex / 3.0, maxTriangleVertex / 3.0);
        glScaled(triangleVertex, triangleVertex, triangleVertex);
        glPushMatrix();
        {
            glTranslatef(-1 / 3.0, -1 / 3.0, -1 / 3.0);
            drawTriangle();
        }
        glPopMatrix();
        glPopMatrix();
    }

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glColor3f(i % 2, (i + 1) % 2, 1.0f);
            glRotatef(90 * i, 0, 0, 1);
            glRotatef(180, 1, 1, 0);
            glTranslatef(maxTriangleVertex / 3.0, maxTriangleVertex / 3.0, maxTriangleVertex / 3.0);
            glScaled(triangleVertex, triangleVertex, triangleVertex);
            glPushMatrix();
            {
                glTranslatef(-1 / 3.0, -1 / 3.0, -1 / 3.0);
                drawTriangle();
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
}

void drawCylinderPart(double radius, double height, int segments = 100)
{
    struct Vec3D points[segments + 1];

    double offset = radians(70.5287794);

    for (int i = 0; i <= segments; i++)
    {
        double theta = (0 - offset) / 2 + offset * (1.0 * i / segments);
        points[i].x = radius * cos(theta);
        points[i].y = radius * sin(theta);
    }

    glBegin(GL_QUADS);
    for (int i = 0; i < segments; i++)
    {
        glVertex3f(points[i].x, points[i].y, height / 2);
        glVertex3f(points[i].x, points[i].y, -height / 2);
        glVertex3f(points[i + 1].x, points[i + 1].y, -height / 2);
        glVertex3f(points[i + 1].x, points[i + 1].y, height / 2);
    }
    glEnd();
}

void drawCylinders()
{
    glColor3f(1.0f, 1.0f, 0.0f);
    double diff = maxTriangleVertex - triangleVertex;
    double radius = (diff / sqrt(2)) * cos(radians(70.5287794 / 2));
    // Each side of triangle has length = triangleVertex * sqrt(2)
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glRotatef((45 + i * 90), 0, 1, 0);
        glTranslatef(triangleVertex / sqrt(2), 0, 0);
        drawCylinderPart(radius, triangleVertex * sqrt(2));
        glPopMatrix();
    }

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glRotatef((45 + i * 90), 0, 1, 0);
        glTranslatef(triangleVertex / sqrt(2), 0, 0);
        drawCylinderPart(radius, triangleVertex * sqrt(2));
        glPopMatrix();
    }
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glRotatef(90, 0, 0, 1);
        glRotatef((45 + i * 90), 0, 1, 0);
        glTranslatef(triangleVertex / sqrt(2), 0, 0);
        drawCylinderPart(radius, triangleVertex * sqrt(2));
        glPopMatrix();
    }
}

void drawSphereQuad(double radius, int segments)
{
    struct Vec3D points[segments + 1][segments + 1];
    int i, j;
    double x, y;
    // generate points
    for (i = 0; i <= segments; i++)
    {
        for (j = 0; j <= segments; j++)
        {
            // Create square from (-1,-1,1) to (1,1,1)
            points[i][j].x = -1 + (double)i / segments * 2;
            points[i][j].y = -1 + (double)j / segments * 2;
            points[i][j].z = 1;

            // Then normalize the radius vectors
            points[i][j] = points[i][j].normalize();
            points[i][j] = points[i][j] * radius;
        }
    }
    // draw quads using generated points
    for (i = 0; i < segments; i++)
    {
        for (j = 0; j < segments; j++)
        {
            glBegin(GL_QUADS);
            {
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
            }
            glEnd();
        }
    }
}

double angleRotationZ = 45.0;

void drawSpheres()
{
    double diff = maxTriangleVertex - triangleVertex;
    double radius = (diff / sqrt(2)) * cos(radians(70.5287794 / 2));
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glColor3f(0, i % 2, (i + 1) % 2); // blue / green
            glRotatef(90 * i, 0, 1, 0);
            glTranslatef(0, 0, triangleVertex);
            drawSphereQuad(radius, 10);
        }
        glPopMatrix();
    }

    for (int i = 0; i < 2; i++)
    {

        glPushMatrix();
        {
            glColor3f(1.0f, 0.0f, 0.0f); // red
            glRotatef(90 + 180 * i, 1, 0, 0);
            glTranslatef(0, 0, triangleVertex);
            drawSphereQuad(radius, 10);
        }
        glPopMatrix();
    }
}

void DGraphics::display()
{
    camera->set();
    // Clear all pixels
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotatef(angleRotationZ, 0, 0, 1);
    drawPyramids();
    drawCylinders();
    drawSpheres();
    glPopMatrix();

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
    switch (key)
    {
    case '1':
        camera->rotateLeft();
        break;
    case '2':
        camera->rotateRight();
        break;
    case '3':
        camera->rotateUp();
        break;
    case '4':
        camera->rotateDown();
        break;
    case '5':
        camera->tiltClock();
        break;
    case '6':
        camera->tiltCounterClock();
        break;
    case 'w':
        camera->moveUpRef();
        break;
    case 's':
        camera->moveDownRef();
        break;

    case 'd':
        angleRotationZ += 5.0f;
        break;
    case 'a':
        angleRotationZ -= 5.0f;
        break;

    case ',':
        triangleVertex -= 0.1;
        if (triangleVertex < 0)
        {
            triangleVertex = 0;
        }
        break;
    case '.':
        triangleVertex += 0.1;
        if (triangleVertex > maxTriangleVertex)
        {
            triangleVertex = maxTriangleVertex;
        }
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
    switch (key)
    {
    case GLUT_KEY_UP:
        camera->moveForward();
        break;
    case GLUT_KEY_DOWN:
        camera->moveBackward();
        break;

    case GLUT_KEY_RIGHT:
        camera->moveRight();
        break;

    case GLUT_KEY_LEFT:
        camera->moveLeft();
        break;

    case GLUT_KEY_PAGE_UP:
        camera->moveUp();
        break;
    case GLUT_KEY_PAGE_DOWN:
        camera->moveDown();
        break;

    default:
        break;
    }
}

void DGraphics::mouseScroll(int dir)
{
    if (dir == 1)
    {
        camera->moveForward();
    }
    else if (dir == -1)
    {
        camera->moveBackward();
    }
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv); // Initialize GLUT // Without this : Other glut functions can't be called .
    DGraphics::initialize(640, 640, "Magic Cube");
    glutMainLoop(); // Enter the event-processing loop
    return 0;
}