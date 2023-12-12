#include "DGraphics.hpp"
#include "Headers.hpp"
GLint DGraphics::WindowHeight = 600;
GLint DGraphics::WindowWidth = 600;

void DGraphics::drawCheckerboard()
{
    int size = 100;
    for (int i = -size; i < size; i += 2)
    {
        for (int j = abs(i) - size; j < size - abs(i); j += 2)
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

void DGraphics::drawSquare(double a)
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

void DGraphics::drawAxis()
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
void DGraphics::drawCube()
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

void DGraphics::drawCylinder(float radius, float height)
{
    int numSegments = 50;
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= numSegments; ++i)
    {
        float theta = 2.0 * M_PI * static_cast<float>(i) / numSegments;
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        glVertex3f(x, y, 0.0);
        glVertex3f(x, y, height);
    }
    glEnd();
}

void DGraphics::drawCone(float radius, float height)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0, 0.0, height); // Apex
    int numSegments = 50;
    for (int i = 0; i <= numSegments; ++i)
    {
        float theta = 2.0 * M_PI * static_cast<float>(i) / numSegments;
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        glVertex3f(x, y, 0.0);
    }
    glEnd();
}
void DGraphics::drawArrow(double length)
{
    drawCylinder(0.05, length);
    glTranslatef(0.0, 0.0, length); // Move the cone up to connect with the cylinder
    drawCone(0.1, 0.5);
}

void DGraphics::keyHandler(unsigned char key, int, int) // Callback When normal key is pressed down
{
    keyboard(key);
    glutPostRedisplay();
}
void DGraphics::specialKeyHandler(int key, int, int) // Callback when special key is pressed down
{
    specialKeyboard(key);
    glutPostRedisplay();
}
void DGraphics::displayHandler()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Ensures that each time when we enter the projection mode, the matrix will be reset to identity matrix
    display();
    glutSwapBuffers();
}

void DGraphics::mouseClickHandler(int button, int state, int x, int y)
{
    if (button == 3)
    {
        mouseScroll(1);
    }
    else if (button == 4)
    {
        mouseScroll(-1);
    }

    glutPostRedisplay();
}

void DGraphics::resize(int w, int h)
{
    glViewport(0, 0, w, h); // resizing the window

    // Setup viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    WindowWidth = w;
    WindowHeight = h;

    gluPerspective(
        60,          // fov
        1.0 * w / h, // aspect
        1,
        200);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void DGraphics::initialize(int width, int height, char *title)
{
    WindowWidth = width;
    WindowHeight = height;
    // Initialize GLUT // Without this : Other glut functions can't be called .
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(500, 0);                // Position the window's initial top-left corner
    glutInitWindowSize(WindowWidth, WindowHeight); // Set the window's initial width & height // Default value : 300,300
    glutCreateWindow(title);                       // Create a window with the given tit le	// Without this : ERROR:  glutMainLoop called with no windows created.
    glutDisplayFunc(DGraphics::displayHandler);    // Register display callback handler for window re-paint  // Without this: ERROR:  No display callback registered for window 1
    glutReshapeFunc(DGraphics::resize);

    // Input handlers
    glutKeyboardFunc(DGraphics::keyHandler);
    glutSpecialFunc(DGraphics::specialKeyHandler); // special keys
    glutMouseFunc(DGraphics::mouseClickHandler);

    // Anim

    // glutTimerFunc(10, collision, 0);

    glEnable(GL_DEPTH_TEST); // je object kache setake rakhe
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(3.0);
    glEnable(GL_LINE_SMOOTH);
    DGraphics::init();
    // glutFullScreen();
    // glutSetCursor(GLUT_CURSOR_NONE);
    // Enter the event-processing loop
}