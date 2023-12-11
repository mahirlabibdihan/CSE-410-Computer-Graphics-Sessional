
#ifdef __linux__
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <stdio.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

#include "Point.hpp"
#include "Camera.hpp"
#include "Draw.hpp"
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define PI 3.14159265358979324
#define xyz(r, a, t) r *cos(a) * cos(t), r *cos(a) * sin(t), r *sin(a)
#define color(R, G, B) .3 + R / (1.3 * r), .3 + G / (1.3 * r), .3 + B / (1.3 * r)
GLint WindowHeight = 600;
GLint WindowWidth = 600;
Camera camera;

double maxTriangleLength = 1.6;
double triangleLength = 1.6;

double maxSphereRadius = maxTriangleLength / sqrt(3.0);
double sphereRadius = 0;
double sphereStep = maxSphereRadius / 16.0;

void drawAxes()
{
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(2, 0, 0); // Red
    // X axis
    glVertex3f(0, 0, 0);
    glVertex3f(2, 0, 0);

    glColor3f(0, 2, 0); // Green
    // Y axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 2, 0);

    glColor3f(0, 0, 2); // Blue
    // Z axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 2);
    glEnd();
}

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
void drawGrid()
{
    int i;

    glColor3f(0.6, 0.6, 0.6); // grey
    glBegin(GL_LINES);
    {
        for (i = -18; i <= 18; i++)
        {

            // if (i == 0)
            //     continue; // SKIP the MAIN axes

            // lines parallel to Y-axis
            glVertex3f(i * 0.1, -1.9, 0);
            glVertex3f(i * 0.1, 1.9, 0);

            // lines parallel to X-axis
            glVertex3f(-1.9, i * 0.1, 0);
            glVertex3f(1.9, i * 0.1, 0);
        }
    }
    glEnd();
}

void drawPyramids()
{
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glColor3f((i + 1) % 2, i % 2, 1.0f); // purple / cyan
        glRotatef(90 * i, 0, 0, 1);
        glTranslatef(maxTriangleLength / 3.0, maxTriangleLength / 3.0, maxTriangleLength / 3.0);
        glScaled(triangleLength, triangleLength, triangleLength);
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
            glColor3f(i % 2, (i + 1) % 2, 1.0f); // purple / cyan
            glRotatef(90 * i, 0, 0, 1);
            glRotatef(180, 1, 1, 0);
            glTranslatef(maxTriangleLength / 3.0, maxTriangleLength / 3.0, maxTriangleLength / 3.0);
            glScaled(triangleLength, triangleLength, triangleLength);
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
    struct PT points[segments + 1];

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
    double diff = maxTriangleLength - triangleLength;
    double radius = (diff / sqrt(2)) * cos(radians(70.5287794 / 2));
    // Each side of triangle has length = triangleLength * sqrt(2)
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glRotatef((45 + i * 90), 0, 1, 0);
        glTranslatef(triangleLength / sqrt(2), 0, 0);
        drawCylinderPart(radius, triangleLength * sqrt(2));
        glPopMatrix();
    }

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glRotatef((45 + i * 90), 0, 1, 0);
        glTranslatef(triangleLength / sqrt(2), 0, 0);
        drawCylinderPart(radius, triangleLength * sqrt(2));
        glPopMatrix();
    }
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glRotatef(90, 0, 0, 1);
        glRotatef((45 + i * 90), 0, 1, 0);
        glTranslatef(triangleLength / sqrt(2), 0, 0);
        drawCylinderPart(radius, triangleLength * sqrt(2));
        glPopMatrix();
    }
}

// double dot(PT a, PT b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
double length(PT a) { return sqrt(dot(a, a)); }
PT normalize(PT a) { return a / length(a); }
void drawSphereQuad(double radius, int segments)
{
    struct PT points[segments + 1][segments + 1];
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
            points[i][j] = normalize(points[i][j]);
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
    double diff = maxTriangleLength - triangleLength;
    double radius = (diff / sqrt(2)) * cos(radians(70.5287794 / 2));
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glColor3f(0, i % 2, (i + 1) % 2); // blue / green
            glRotatef(90 * i, 0, 1, 0);
            glTranslatef(0, 0, triangleLength);
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
            glTranslatef(0, 0, triangleLength);
            drawSphereQuad(radius, 10);
        }
        glPopMatrix();
    }
}

void display()
{

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Ensures that each time when we enter the projection mode, the matrix will be reset to identity matrix
    camera.setCamera();
    // Clear all pixels
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawAxes();
    // drawGrid();

    glRotatef(angleRotationZ, 0, 0, 1);
    drawPyramids();
    drawCylinders();
    drawSpheres();

    glutSwapBuffers();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h); // resizing the window

    // Setup viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    WindowWidth = w;
    WindowHeight = h;
    // L      R     B    T      N       F
    // glOrtho(-WindowWidth / 2, WindowWidth / 2, -WindowHeight / 2, WindowHeight / 2, -WindowWidth / 2, WindowWidth / 2);
    gluPerspective(
        60,          // fov
        1.0 * w / h, // aspect
        // 1,
        1,
        200);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void update(int time)
{
    // angle = (angle + 1) % 360;
    // printf("%d\n", dir_angle);
    glutPostRedisplay();
    glutTimerFunc(time, update, time);
}

bool last_i = true;
void key(unsigned char key, int, int)
{
    // if (key == 27) glutLeaveMainLoop();
    if (key == 27)
        glutLeaveMainLoop();
    switch (key)
    {
    case '1':
        camera.rotateLeft();
        break;
    case '2':
        camera.moveRight();
        break;
    case '3':
        camera.rotateUp();
        break;
    case '4':
        camera.rotateDown();
        break;
    case '5':
        camera.tiltClock();
        break;
    case '6':
        camera.tiltCounterClock();
        break;
    case 'w':
        camera.moveUpRef();
        break;
    case 's':
        camera.moveDownRef();
        break;

    case 'd':
        angleRotationZ -= 5.0f;
        break;
    case 'a':
        angleRotationZ += 5.0f;
        break;

    case ',':
        triangleLength -= 0.01;
        // sphereRadius += sphereStep;
        if (triangleLength < 0)
        {
            triangleLength = 0;
            sphereRadius = maxSphereRadius;
        }
        break;
    case '.':
        triangleLength += 0.01;
        // sphereRadius -= sphereStep;
        if (triangleLength > maxTriangleLength)
        {
            triangleLength = maxTriangleLength;
            sphereRadius = 0;
        }
        break;
    }
}
void specialkey(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        camera.moveForward();
        break;
    case GLUT_KEY_DOWN:
        camera.moveBackward();
        break;

    case GLUT_KEY_RIGHT:
        camera.moveRight();
        break;

    case GLUT_KEY_LEFT:
        camera.moveLeft();
        break;

    case GLUT_KEY_PAGE_UP:
        camera.moveUp();
        break;
    case GLUT_KEY_PAGE_DOWN:
        camera.moveDown();
        break;

    default:
        break;
    }

    glutPostRedisplay();
}

// void mouseDrag(int mx, int my)
// {
//     // iMouseX = mx;
//     // iMouseY = iScreenHeight - my;
//     // iMouseDrag(iMouseX, iMouseY);
//     glutSwapBuffers();
// }

// void mouseMove(int mx, int my)
// {
//     // iMouseDirection(mx, iScreenHeight - my);
//     // iMouseX = mx;
//     // iMouseY = iScreenHeight - my;
//     // iMouseMove(iMouseX, iMouseY);

//     glutSwapBuffers();
// }

void mouseClick(int button, int state, int x, int y)
{
    // iMouseX = x;
    // iMouseY = iScreenHeight - y;
    // iMouseClick(button, state, iMouseX, iMouseY);
    if (button == 3)
    {
        camera.moveForward();
    }
    else if (button == 4)
    {
        camera.moveBackward();
    }

    glutPostRedisplay();
    // glutSwapBuffers();
}
// void mouseScroll(int button, int dir, int x, int y)
// {
//     // iMouseScroll(dir);

//     // glutPostRedisplay();
// }

int main(int argc, char **argv)
{
    glutInit(&argc, argv); // Initialize GLUT // Without this : Other glut functions can't be called .
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(500, 0);                // Position the window's initial top-left corner
    glutInitWindowSize(WindowWidth, WindowHeight); // Set the window's initial width & height // Default value : 300,300
    glutCreateWindow("First Program");             // Create a window with the given tit le	// Without this : ERROR:  glutMainLoop called with no windows created.
    glutDisplayFunc(display);                      // Register display callback handler for window re-paint  // Without this: ERROR:  No display callback registered for window 1
    glutReshapeFunc(resize);

    // Input handlers
    glutKeyboardFunc(key);
    glutSpecialFunc(specialkey); // special keys
    glutMouseFunc(mouseClick);
    // glutMotionFunc(mouseDrag);
    // glutPassiveMotionFunc(mouseMove);
    // glutMouseWheelFunc(mouseScroll);

    glutTimerFunc(10, update, 10);
    // glMatrixMode(GL_MODELVIEW);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        60, // fov
        // 1920/1080.0, // aspect
        1,
        1,
        200);

    // gluPerspective(80,	1,	1,	1000.0);

    // glOrtho(-WindowWidth / 2, WindowWidth / 2, -WindowHeight / 2, WindowHeight / 2, -WindowWidth / 2, WindowWidth / 2);

    // GLfloat mat_specular[] = {1.0, 1.0, 1.0};
    // GLfloat mat_shininess[] = {50.0};
    // GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    // glClearColor(0, 0, 0, 0);
    // glShadeModel(GL_SMOOTH);
    // glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    // glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    // glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // glEnable(GL_COLOR_MATERIAL);
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_LINE_SMOOTH);

    glEnable(GL_DEPTH_TEST); // je object kache setake rakhe
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.0);
    glEnable(GL_LINE_SMOOTH);
    init();
    // glutFullScreen();
    // glutSetCursor(GLUT_CURSOR_NONE);
    glutMainLoop(); // Enter the event-processing loop
    return 0;
}