
#ifdef __linux__
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include "Draw.hpp"
#include "Camera.hpp"
#include "Ball.hpp"
#include "Wall.hpp"
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

Camera camera;
Ball ball;
using namespace std;
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define PI 3.14159265358979324
#define xyz(r, a, t) r *cos(a) * cos(t), r *cos(a) * sin(t), r *sin(a)
#define color(R, G, B) .3 + R / (1.3 * r), .3 + G / (1.3 * r), .3 + B / (1.3 * r)
GLint WindowHeight = 600;
GLint WindowWidth = 600;
vector<PT> generate_equilateral_polygon(double center_x, double center_y, double radius, int num_sides)
{
    vector<PT> vertices;
    for (int i = 0; i < num_sides; ++i)
    {
        double angle = (45 * PI / 180) + 2.0 * PI * i / num_sides;
        double x = center_x + radius * cos(angle);
        double y = center_y + radius * sin(angle);
        vertices.push_back({x, y, 0});
    }
    return vertices;
}

bool animate = false;

int stacks = 5, sectors = 3;

int r = 300;

void printPT(PT a) { printf("(%lf,%lf,%lf)\n", a.x, a.y, a.z); }

bool last_i = true;

bool predicted = false;

void display()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Ensures that each time when we enter the projection mode, the matrix will be reset to identity matrix
    camera.setCamera();

    // Clear all pixels
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    drawCheckerboard();

    glDisable(GL_DEPTH_TEST); // je object kache setake rakhe
    glPushMatrix();
    drawWall();
    glPopMatrix();
    glEnable(GL_DEPTH_TEST); // je object kache setake rakhe
    // Ball
    glPushMatrix();
    // glTranslatef(ball.x, ball.y, 0);
    // glPushMatrix();
    glTranslatef(ball.pos.x, ball.pos.y, ball.pos.z);

    ball.draw(0.5, 8, 40);
    // glPopMatrix();
    glPopMatrix();

    // Direction arrow
    glPushMatrix();
    glTranslatef(ball.pos.x, ball.pos.y, ball.pos.z);
    glRotatef(fmod((atan2(ball.look.y, ball.look.x) * 180 / PI) + 360.0, 360.0), ball.up.x, ball.up.y, ball.up.z);
    // glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glColor3f(0.0, 0.0, 1.0); // Red color for the cylinder
    drawArrow(1.0);
    // glPopMatrix();
    glPopMatrix();

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
    double center_x = 0.0;
    double center_y = 0.0;
    double radius = sqrt(72);

    ball.init(0.5, 8, 40);
    // Define the number of sides
    // int num_sides = 5;
    vertex = generate_equilateral_polygon(center_x, center_y, radius, n);
}

double getNearestWall(bool back)
{
    double dist = INT64_MAX;
    int idx = -1;
    for (int i = 0; i < n; i++)
    {
        PT tmp_look = ball.look * (back ? -1 : 1);
        PT A = vertex[i];
        PT B = vertex[(i + 1) % n];
        PT C = ball.pos;
        PT D = ball.pos + (tmp_look * 100);
        // if (intersect(A, B, C, D))
        {
            PT dir = B - A;

            double theta = acos(dot2D(dir, tmp_look) / (det2D(dir) * det2D(tmp_look)));

            // printf("Wall: %d, Theta: %lf\n", i, theta * 180 / PI);

            PT I = findIntersectionInf(A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);
            // printPT(I);
            if (isnanf(I.x) || isnanf(I.y))
            {
                continue;
            }

            PT tmp = I - C;

            // printPT(ball.look);
            // printPT(tmp);
            double check_theta = acos(min(max(dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)), -1.0), 1.0));

            // printf("Check: %lf %d\n", check_theta, std::fabs(check_theta) < 1e-9);
            if (floor(check_theta) == 0 || back)
            {
                double distance = distancePointToPoint(C, I);
                double minus = ball.radius / sin(theta);
                if (dist > distance - minus)
                {
                    idx = i;
                    dist = distance - minus;
                }
            }
        }
    }
    return idx;
}

void handleCollision(bool back)
{
    // printf("==============================Collision=================================\n");
    // int n = 4;

    int i = getNearestWall(back);
    // for (int i = 0; i < n; i++)
    {
        PT A = vertex[i];
        PT B = vertex[(i + 1) % n];
        PT C = ball.pos;
        PT D = ball.pos + (ball.look * 100);
        // if (intersect(A, B, C, D))
        {
            PT direction_vector = A - B;
            PT orthogonal_direction_vector = PT(-direction_vector.y, direction_vector.x, 0);
            orthogonal_direction_vector = orthogonal_direction_vector / sqrt(dot(orthogonal_direction_vector, orthogonal_direction_vector));
            ball.look = ball.look - orthogonal_direction_vector * 2 * dot(ball.look, orthogonal_direction_vector);
            ball.right = PT(ball.look.y, -ball.look.x, 0); // Orthogonal
        }
    }
    // printf("==============================Collision=================================\n");
}

void predictCollision()
{
    // printf("==============================Collision=================================\n");
    // int n = 4;
    int i = getNearestWall(false);
    // for (int i = 0; i < n; i++)
    {
        PT A = vertex[i];
        PT B = vertex[(i + 1) % n];
        PT C = ball.pos;
        PT D = ball.pos + (ball.look * 100);
        // if (intersect(A, B, C, D))
        {
            PT direction_vector = A - B;
            PT orthogonal_direction_vector = PT(-direction_vector.y, direction_vector.x, 0);
            orthogonal_direction_vector = orthogonal_direction_vector / sqrt(dot(orthogonal_direction_vector, orthogonal_direction_vector));
            ball.look = ball.look - orthogonal_direction_vector * 2 * dot(ball.look, orthogonal_direction_vector);
            ball.right = PT(ball.look.y, -ball.look.x, 0); // Orthogonal
            // break;
        }
    }
}

double getMinDistance(bool back)
{
    double dist = INT64_MAX;
    for (int i = 0; i < n; i++)
    {
        PT tmp_look = ball.look * (back ? -1 : 1);
        PT A = vertex[i];
        PT B = vertex[(i + 1) % n];
        PT C = ball.pos;
        PT D = ball.pos + (tmp_look * 100);
        // if (intersect(A, B, C, D))
        {
            PT dir = B - A;
            double theta = acos(dot2D(dir, tmp_look) / (det2D(dir) * det2D(tmp_look)));

            // printf("Wall: %d, Theta: %lf\n", i, theta * 180 / PI);

            PT I = findIntersectionInf(A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);

            if (isnanf(I.x) || isnanf(I.y))
            {
                continue;
            }

            PT tmp = I - C;

            double check_theta = acos(min(max(dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)), -1.0), 1.0));

            if (floor(check_theta) == 0)
            {
                double distance = distancePointToPoint(C, I);
                double minus = ball.radius / sin(theta);
                printf("%lf\n", ball.radius);
                dist = min(dist, distance - minus);

                printf("Dist: %lf, fULL: %lf, N: %lf, D: %lf, %lf\n", distance - minus, distance, dot2D(tmp, tmp_look), det2D(tmp) * det2D(tmp_look), dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)));
            }
        }
    }
    return dist;
}

bool checkCollision(bool back)
{
    double min_dist = getMinDistance(back);
    printf("Min_Dist: %lf\n", min_dist);
    if (min_dist <= 1e-9)
    {
        printf("Collusion\n");
        return true;
    }
    return false;
}

int counter = 0;
void update(int time)
{
    counter += time;
    if (animate)
    {
        double d = min(getMinDistance(false), 0.25);
        ball.pos = ball.pos + (ball.look * d);
        ball.rotate(-25 * d / 0.25, 8, 40);
    }
    glutPostRedisplay();
    glutTimerFunc(time, update, time);
}

int current_id = 0;

void collision(int id)
{
    // printf("Callback %d %d\n", id, current_id);
    int time = 10;
    if (id == current_id)
    {
        if (animate)
        {
            if (predicted)
            {
                predictCollision();
            }
            else
            {
                current_id++;
                id = current_id;
            }
            // predictCollision();
            double min_dist = getMinDistance(false);
            time = min_dist / (det2D(ball.look) * 0.25 / 100);

            counter = 0;
            predicted = true;
            printf("Callback at %d, id: %d\n", time, id);
            glutPostRedisplay();
            glutTimerFunc(max(time, 0), collision, id);
        }
    }
}

bool key_state[128];

void key(unsigned char key, int, int)
{
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

    case 'i':
        if (!animate)
        {
            ball.pos = ball.pos + (ball.look * min(0.2, getMinDistance(false)));
            ball.rotate(-20, 8, 40);
            if (checkCollision(false))
            {
                handleCollision(false);
            }
        }
        break;

    case 'k':
        if (!animate)
        {
            ball.pos = ball.pos - (ball.look * min(0.2, getMinDistance(true)));
            ball.rotate(20, 8, 40);
            if (checkCollision(true))
            {
                handleCollision(true);
            }
        }
        break;

    case 'a':
    case 'j':
        rotate3D(ball.right, ball.up, radians(15));
        rotate3D(ball.look, ball.up, radians(15));
        predicted = false;
        if (animate)
            glutTimerFunc(0, collision, ++current_id);
        // printf("%lf %lf\n",atan(ball.look.y / ball.look.x), fmod((atan2(ball.look.y, ball.look.x)* 180 / PI)+360.0,360.0));
        break;

    case 'd':
    case 'l':

        rotate3D(ball.right, ball.up, -radians(15));
        rotate3D(ball.look, ball.up, -radians(15));
        predicted = false;
        // getMinDistance();f
        if (animate)
            glutTimerFunc(0, collision, ++current_id);
        // printf("%lf %lf\n",atan(ball.look.y / ball.look.x), fmod((atan2(ball.look.y, ball.look.x)* 180 / PI)+360.0,360.0));
        break;
    case ' ':
        animate = !animate;
        if (animate)
        {
            predicted = false;
            glutTimerFunc(0, collision, ++current_id);
        }
        break;

    case '0':
        n = min(n + 1, 20);
        ball.pos.x = 0;
        ball.pos.y = 0;
        predicted = false;
        if (animate)
            glutTimerFunc(0, collision, ++current_id);
        {
            double center_x = 0.0;
            double center_y = 0.0;
            double radius = sqrt(72);

            // Define the number of sides
            // int num_sides = 5;
            vertex = generate_equilateral_polygon(center_x, center_y, radius, n);
            break;
        }
    case '9':
        n = max(3, n - 1);
        ball.pos.x = 0;
        ball.pos.y = 0;
        predicted = false;
        if (animate)
            glutTimerFunc(0, collision, ++current_id);
        {
            double center_x = 0.0;
            double center_y = 0.0;
            double radius = sqrt(72);

            // Define the number of sides
            // int num_sides = 5;
            vertex = generate_equilateral_polygon(center_x, center_y, radius, n);
        }

        break;
    }
    glutPostRedisplay();
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

void mouseDrag(int mx, int my)
{
    // iMouseX = mx;
    // iMouseY = iScreenHeight - my;
    // iMouseDrag(iMouseX, iMouseY);
    glutSwapBuffers();
}

void mouseMove(int mx, int my)
{
    // iMouseDirection(mx, iScreenHeight - my);
    // iMouseX = mx;
    // iMouseY = iScreenHeight - my;
    // iMouseMove(iMouseX, iMouseY);

    glutSwapBuffers();
}

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

void mouseScroll(int button, int dir, int x, int y)
{
    // iMouseScroll(dir);

    // glutPostRedisplay();
}

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
    glutMouseWheelFunc(mouseScroll);
    glutTimerFunc(100, update, 100);
    // glutTimerFunc(10, collision, 0);
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
    // glEnable(GL_LIGHT0);ez
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_LINE_SMOOTH);

    glEnable(GL_DEPTH_TEST); // je object kache setake rakhe
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(3.0);
    glEnable(GL_LINE_SMOOTH);
    init();
    // glutFullScreen();
    // glutSetCursor(GLUT_CURSOR_NONE);
    glutMainLoop(); // Enter the event-processing loop
    return 0;
}