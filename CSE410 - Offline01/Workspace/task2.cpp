
#ifdef __linux__
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

using namespace std;
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define PI 3.14159265358979324
#define xyz(r, a, t) r *cos(a) * cos(t), r *cos(a) * sin(t), r *sin(a)
#define color(R, G, B) .3 + R / (1.3 * r), .3 + G / (1.3 * r), .3 + B / (1.3 * r)
GLint WindowHeight = 600;
GLint WindowWidth = 600;
int angle = 0;

int dir_angle = 30;
double ball_rot = 0, ball_rot_x = 0, ball_rot_y = 0;
double ball_x = 0, ball_y = 0;
double ROT_ANG = PI / 180;

struct PT
{
    PT() {}
    double x, y, z;

    PT(double x, double y, double z) : x(x), y(y), z(z) {}
    PT(const PT &p) : x(p.x), y(p.y), z(p.z) {}

    // arithemtic operations
    PT operator+(PT b) { return PT(x + b.x, y + b.y, z + b.z); }
    PT operator-(PT b) { return PT(x - b.x, y - b.y, z - b.z); }
    PT operator*(double b) { return PT(x * b, y * b, z * b); }
    PT operator/(double b) { return PT(x / b, y / b, z / b); }
    PT operator/(PT b) { return PT(x / b.x, y / b.y, z / b.y); }
    PT operator*(PT b) { return PT(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x); }
};

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

int n = 4;
vector<PT> vertex;
// = {PT(6,6,0),PT(-6,6,0),PT(-6,-6,0),PT(6,-6,0)}
bool ccw(PT A, PT B, PT C)
{
    return (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x);
}

bool intersect(PT A, PT B, PT C, PT D)
{
    return ccw(A, C, D) != ccw(B, C, D) and ccw(A, B, C) != ccw(A, B, D);
}

bool animate = false;

PT minX(PT p, PT q)
{
    return p.x < q.x ? p : q;
}
PT maxX(PT p, PT q)
{
    return p.x > q.x ? p : q;
}
PT minY(PT p, PT q)
{
    return p.y < q.y ? p : q;
}
PT maxY(PT p, PT q)
{
    return p.y > q.y ? p : q;
}

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

int stacks = 5, sectors = 3;

int r = 300;

void circle(int x, int y, int r)
{
    GLfloat t, dt = 2 * PI / 100;
    glBegin(GL_LINE_STRIP);
    glLineWidth(3.0);
    // Draw circle
    for (t = 0; t <= 2 * PI + dt; t += dt)
    {
        glVertex2f(x + r * cos(t), y + r * sin(t)); // Formula of circle
    }
    glEnd();
}

double dot(PT a, PT b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
double dot2D(PT a, PT b) { return a.x * b.x + a.y * b.y; }
double printPT(PT a) { printf("(%lf,%lf,%lf)\n", a.x, a.y, a.z); }
void rotate3D(PT &vec, PT &axis, double ang)
{
    vec = vec * cos(ang) + axis * (1 - cos(ang)) * dot(axis, vec) + (axis * vec) * sin(ang);
}
bool last_i = true;

// position of camera
PT pos(10, 10, 10);
// up, right and look direction
PT up(-1 / sqrt(3), -1 / sqrt(3), 1 / sqrt(3));
PT right(-1 / sqrt(2), 1 / sqrt(2), 0);
PT look(-1 / sqrt(3), -1 / sqrt(3), -1 / sqrt(3));

// PT pos(0, 0, 10);
// // up, right and look direction
// PT up(1, 0, 0);
// PT right(0, -1, 0);
// PT look(0, 0, -1);

GLfloat ball_radius = 0.5;
PT ball_look(1 / sqrt(2), 1 / sqrt(2), 0);
PT ball_right(1 / sqrt(2), -1 / sqrt(2), 0);
// PT ball_look(1,0,0);
// PT ball_right(0,-1,0);
PT ball_up(0, 0, 1);
PT ball_pos(0, 0, ball_radius);

struct PT points[100][100];
struct PT lower_points[100][100];
void initBall(double radius, int slices, int stacks)
{
    int i, j;
    double h, r;
    for (i = 0; i <= stacks; i++)
    {
        h = radius * sin(((double)i / (double)stacks) * (PI / 2));
        r = radius * cos(((double)i / (double)stacks) * (PI / 2));
        for (j = 0; j <= slices; j++)
        {
            lower_points[i][j].x = points[i][j].x = r * cos(((double)j / (double)slices) * 2 * PI);
            lower_points[i][j].y = points[i][j].y = r * sin(((double)j / (double)slices) * 2 * PI);
            points[i][j].z = h;
            lower_points[i][j].z = -h;
        }
    }
}
void updateBall(double ang, int slices, int stacks)
{
    int i, j;
    for (i = 0; i <= stacks; i++)
    {
        for (j = 0; j <= slices; j++)
        {
            rotate3D(points[i][j], ball_right, ang * ROT_ANG);
            rotate3D(lower_points[i][j], ball_right, ang * ROT_ANG);
        }
    }
}

void drawSphere(double radius, int slices, int stacks)
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

void sphere()
{
    glPushMatrix();
    glRotatef(angle, 1, 0, 0);
    glRotatef(angle, 0, 1, 0);
    glRotatef(angle, 0, 0, 1);
    GLdouble t, dt = 2 * PI / stacks;
    for (t = 0; t <= 2 * PI; t += dt)
    {
        GLfloat x = r * cos(t), y = r * sin(t);
        glPushMatrix();
        glRotatef(atan(y / x) * 180 / PI, 0, 1, 0);
        glRotatef(90, 0, 0, 1);
        glColor3f(0.0, 0.0, 0.0);
        circle(0, 0, r);
        glPopMatrix();
    }

    dt = 2 * PI / sectors;
    for (t = 0; t <= 2 * PI; t += dt)
    {
        glPushMatrix();
        glTranslatef(0, sin(t) * r, 0);
        glRotatef(90, 1, 0, 0);
        glColor3f(0.0, 0.0, 0.0);
        circle(0, 0, sqrt(pow(r, 2.0) - pow(sin(t) * r, 2.0)));
        glPopMatrix();
    }

    glColor3f(1.0, 1.0, 0.0);

    GLfloat a, da = 2 * PI / sectors;
    for (a = 0; a <= 2 * PI; a += da)
    {
        GLdouble t, dt = 2 * PI / stacks;
        for (t = 0; t <= 2 * PI; t += dt)
        {
            GLfloat vertex[4][3] = {
                {xyz(r, a, t)},
                {xyz(r, a, t + dt)},
                {xyz(r, a + da, t + dt)},
                {xyz(r, a + da, t)}};

            glBegin(GL_POLYGON);
            // glColor3f(color(vertex[0][0],vertex[0][1],vertex[0][2]));
            for (int i = 0; i < 4; i++)
            {
                glColor3f(color(vertex[i][0], vertex[i][1], vertex[i][2]));
                glVertex3fv(vertex[i]);
            }
            glEnd();
        }
    }
    glPopMatrix();
}

// void drawSphereQuad(double radius, int slices, int stacks, double R, double G, double B)
// {
//     struct PT points[100][100];
//     int i, j;
//     double h, r;
//     // generate points
//     GLfloat a, da = 2 * PI / stacks;
//     for (i = 0; i <= stacks; i++)
//     {
//         h = radius * sin(((double)i / stacks) * (PI / 2));
//         r = radius * cos(((double)i / stacks) * (PI / 2));
//         for (j = 0; j < 2; j++)
//         {
//             points[i][j].x = r * cos(((double)j / (double)slices) * 2 * PI);
//             points[i][j].y = r * sin(((double)j / (double)slices) * 2 * PI);
//             points[i][j].z = h;
//         }
//     }

//     // draw quads using generated points
//     for (i = 0; i < stacks; i++)
//     {
//         // glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
//         glColor3f(R, G, B);
//         for (j = 0; j < 1; j++)
//         {
//             glBegin(GL_QUADS);
//             {
//                 // upper hemisphere
//                 glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
//                 glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
//                 glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
//                 glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
//                 // //lower hemisphere
//                 // glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
//                 // glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
//                 // glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
//                 // glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
//             }
//             glEnd();
//         }
//     }
// }

bool predicted = false;
void wall()
{
    glBegin(GL_QUADS);
    {
        glColor4f(1, 0, 0, 1);
        glVertex3f(+6, 0, 0.0);
        glVertex3f(+6, 0, 0.5);
        glVertex3f(-6, 0, 0.5);
        glVertex3f(-6, 0, 0.0);
    }
    glEnd();
}

void fullwall()
{
    glBegin(GL_QUADS);
    {

        glColor4f(1, 0, 0, 1);
        for (int i = 0; i < n; i++)
        {
            glVertex3f(vertex[i].x, vertex[i].y, 0.0);
            glVertex3f(vertex[(i + 1) % n].x, vertex[(i + 1) % n].y, 0.0);
            glVertex3f(vertex[(i + 1) % n].x, vertex[(i + 1) % n].y, 0.5);
            glVertex3f(vertex[i].x, vertex[i].y, 0.5);
        }
    }
    glEnd();
}
// void ball()
// {
//     // glPushMatrix();
//     // glTranslatef(0 , 0, 0.5);
//     glPushMatrix();
//     drawSphereQuad(0.5, 8, 40, 1, 0, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 0, 1, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 1, 0, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 0, 1, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 1, 0, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 0, 1, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 1, 0, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 0, 1, 0);
//     glPopMatrix();

//     glPushMatrix();
//     glRotatef(180, 1, 0, 0);
//     glPushMatrix();
//     drawSphereQuad(0.5, 8, 40, 1, 0, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 0, 1, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 1, 0, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 0, 1, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 1, 0, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 0, 1, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 1, 0, 0);
//     glRotatef(45, 0, 0, 1);
//     drawSphereQuad(0.5, 8, 40, 0, 1, 0);
//     glPopMatrix();
//     glPopMatrix();
//     // glPopMatrix();
// }

double ball_r = 0;
void drawArrow()
{
    // Draw arrow body
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.2f);
    glVertex2f(1.0f, -0.2f);
    glEnd();

    // Draw arrowhead
    glBegin(GL_TRIANGLES);
    glVertex2f(1.0f, 0.1f);
    glVertex2f(1.2f, 0.0f);
    glVertex2f(1.0f, -0.1f);
    glEnd();
}

const int numSegments = 50;
void drawCylinder(float radius, float height)
{
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= numSegments; ++i)
    {
        float theta = 2.0 * PI * static_cast<float>(i) / numSegments;
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
        float theta = 2.0 * PI * static_cast<float>(i) / numSegments;
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        glVertex3f(x, y, 0.0);
    }
    glEnd();
}

int ball_rot_z = 0;
float ball_rot_right = 0;
void display()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Ensures that each time when we enter the projection mode, the matrix will be reset to identity matrix
    gluLookAt(pos.x, pos.y, pos.z,
              pos.x + look.x, pos.y + look.y, pos.z + look.z,
              // ball_pos.x , ball_pos.y, ball_pos.z,
              up.x, up.y, up.z);
    // gluLookAt(
    //     -8, -8, 10,  // EYE: Location of camera
    //     0, 0, 0, // AT: direction where the camera is aiming at.
    //     0, 0, 1 // UP: upward orientation of the camera rougly. typically coincided with y axis
    // );

    // Clear all pixels
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    drawCheckerboard();
    double x[] = {0.5, 0.5, -0.5, -0.5};
    double y[] = {0.5, -0.5, -0.5, 0.5};
    // for(int i=0;i<4;i++)
    // {
    //     glPushMatrix();
    //     // glTranslatef(x[i],y[i],0.0f);
    //     glRotatef(90*i, 1, 0, 0);
    //     glTranslatef(0, 0, 2);

    //     drawSquare(2);
    //     // drawAxis();
    //     // drawCube();
    //     glPopMatrix();
    // }
    // drawAxis();
    // for(int i=0;i<4;i++)
    // {
    //     glPushMatrix();
    //     // glTranslatef(x[i],y[i],0.0f);
    //     glRotatef(90*i, 0, 1, 0);
    //     glTranslatef(0, 0, 2);

    //     drawSquare(2);
    //     // drawAxis();
    //     // drawCube();
    //     glPopMatrix();
    // }

    double axis = dir_angle - 90;

    // for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        // glRotatef(90 * i, 0, 0, 1);
        // glTranslatef(0, 6, 0);
        fullwall();
        glPopMatrix();
    }

    // Ball
    glPushMatrix();
    // glTranslatef(ball_x, ball_y, 0);
    // glPushMatrix();
    glTranslatef(ball_pos.x, ball_pos.y, ball_pos.z);
    // glRotatef(fmod((atan2(ball_look.y, ball_look.x)* 180 / PI)+360.0,360.0), ball_up.x, ball_up.y, ball_up.z);
    // glRotatef(ball_rot_right, ball_right.x, ball_right.y, ball_right.z);
    // ball();
    drawSphere(0.5, 8, 40);
    // glPopMatrix();
    glPopMatrix();

    // Direction arrow
    glPushMatrix();
    glTranslatef(ball_pos.x, ball_pos.y, ball_pos.z);

    glRotatef(fmod((atan2(ball_look.y, ball_look.x) * 180 / PI) + 360.0, 360.0), ball_up.x, ball_up.y, ball_up.z);

    glPushMatrix();
    // glTranslatef(0, 0, 0.5);
    glRotatef(90, 0, 1, 0);
    glColor3f(0.0, 0.0, 1.0); // Red color for the cylinder
    drawCylinder(0.05, 2.0);
    glColor3f(0.0, 0.0, 1.0);    // Blue color for the cone
    glTranslatef(0.0, 0.0, 2.0); // Move the cone up to connect with the cylinder
    drawCone(0.1, 1.0);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(ball_pos.x, ball_pos.y, ball_pos.z);
    glRotatef(fmod((atan2(ball_right.y, ball_right.x) * 180 / PI) + 360.0, 360.0), ball_up.x, ball_up.y, ball_up.z);
    glPushMatrix();
    // glTranslatef(0, 0, 0.5);
    glRotatef(90, 0, 1, 0);
    glColor3f(1.0, 0.0, 0.0); // Red color for the cylinder
    drawCylinder(0.05, 2.0);
    glColor3f(1.0, 0.0, 0.0);    // Blue color for the cone
    glTranslatef(0.0, 0.0, 2.0); // Move the cone up to connect with the cylinder
    drawCone(0.1, 1.0);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(ball_pos.x, ball_pos.y, ball_pos.z);
    glRotatef(-90, 0, 1, 0);
    glPushMatrix();
    {
        // glTranslatef(0, 0, 0.5);
        glRotatef(90, 0, 1, 0);
        glColor3f(0.0, 1.0, 0.0); // Red color for the cylinder
        drawCylinder(0.05, 2.0);
        glColor3f(0.0, 1.0, 0.0);    // Blue color for the cone
        glTranslatef(0.0, 0.0, 2.0); // Move the cone up to connect with the cylinder
        drawCone(0.1, 1.0);
    }

    glPopMatrix();
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

    initBall(0.5, 8, 40);
    // Define the number of sides
    // int num_sides = 5;
    vertex = generate_equilateral_polygon(center_x, center_y, radius, n);
}

// PT findIntersection(double x1, double y1, double x2, double y2,
//                     double x3, double y3, double x4, double y4)
// {
//     PT intersection;

//     double det = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));

//     if (std::abs(det) < 1e-9)
//     {
//         // Lines are parallel or nearly parallel
//         // std::cout << "Lines are parallel. No unique intersection point." << std::endl;
//         intersection.x = NAN;
//         intersection.y = NAN;
//     }
//     else
//     {
//         double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / det;
//         double u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / det;
//         intersection.x = x3 + u * (x4 - x3);
//         intersection.y = y3 + u * (y4 - y3);
//     }

//     return intersection;
// }

PT findIntersectionInf(double x1, double y1, double x2, double y2,
                       double x3, double y3, double x4, double y4)
{
    PT intersection;

    double det = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));

    if (std::abs(det) < 1e-9)
    {
        // Lines are parallel or nearly parallel
        // std::cout << "Lines are parallel. No unique intersection point." << std::endl;
        intersection.x = NAN;
        intersection.y = NAN;
    }
    else
    {
        intersection.x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / det;
        intersection.y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / det;
    }

    return intersection;
}

double distancePointToLine(double x0, double y0, double x1, double y1, double x2, double y2)
{
    double numerator = std::abs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1);
    double denominator = std::sqrt(std::pow(y2 - y1, 2) + std::pow(x2 - x1, 2));
    return numerator / denominator;
}

double det2D(PT v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

double distancePointToPoint(PT a, PT b)
{
    return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

// double getNearestWall()
// {
//     double dist = INT64_MAX;
//     int idx = -1;
//     for (int i = 0; i < n; i++)
//     {
//         PT tmp_look = ball_look;
//         PT A = vertex[i];
//         PT B = vertex[(i + 1) % n];
//         PT C = ball_pos;
//         PT D = ball_pos + (tmp_look * 100);
//         // if (intersect(A, B, C, D))
//         {
//             PT dir = B - A;

//             double theta = acos(dot2D(dir, tmp_look) / (det2D(dir) * det2D(tmp_look)));

//             // printf("Wall: %d, Theta: %lf\n", i, theta * 180 / PI);

//             PT I = findIntersectionInf(A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);
//             // printPT(I);
//             if (isnanf(I.x) || isnanf(I.y))
//             {
//                 continue;
//             }

//             PT tmp = I - C;

//             // printPT(ball_look);
//             // printPT(tmp);
//             double check_theta = acos(min(max(dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)), -1.0), 1.0));

//             // printf("Check: %lf %d\n", check_theta, std::fabs(check_theta) < 1e-9);
//             if (floor(check_theta) == 0)
//             {
//                 double distance = distancePointToPoint(C, I);
//                 double minus = ball_radius / sin(theta);
//                 if (dist > distance - minus)
//                 {
//                     idx = i;
//                     dist = distance - minus;
//                 }

//                 // printf("Dist: %lf, T: %lf, N: %lf, D: %lf, %lf\n", dist, check_theta, dot2D(tmp, tmp_look), det2D(tmp) * det2D(tmp_look), dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)));
//             }
//         }
//     }
//     printf("To wall %d\n", idx);
//     return idx;
// }

double getNearestWallManual(bool back)
{
    double dist = INT64_MAX;
    int idx = -1;
    for (int i = 0; i < n; i++)
    {
        PT tmp_look = ball_look * (back ? -1 : 1);
        PT A = vertex[i];
        PT B = vertex[(i + 1) % n];
        PT C = ball_pos;
        PT D = ball_pos + (tmp_look * 100);
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

            // printPT(ball_look);
            // printPT(tmp);
            double check_theta = acos(min(max(dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)), -1.0), 1.0));

            // printf("Check: %lf %d\n", check_theta, std::fabs(check_theta) < 1e-9);
            if (floor(check_theta) == 0 || back)
            {
                double distance = distancePointToPoint(C, I);
                double minus = ball_radius / sin(theta);
                if (dist > distance - minus)
                {
                    idx = i;
                    dist = distance - minus;
                }

                // printf("Dist: %lf, T: %lf, N: %lf, D: %lf, %lf\n", dist, check_theta, dot2D(tmp, tmp_look), det2D(tmp) * det2D(tmp_look), dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)));
            }
        }
    }
    return idx;
}

void handleCollision(bool back)
{
    // printf("==============================Collision=================================\n");
    // int n = 4;

    int i = getNearestWallManual(back);
    // for (int i = 0; i < n; i++)
    {
        PT A = vertex[i];
        PT B = vertex[(i + 1) % n];
        PT C = ball_pos;
        PT D = ball_pos + (ball_look * 100);
        // if (intersect(A, B, C, D))
        {
            // printf("Collusion at wall %d\n",i);
            // PT X = findIntersection(A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);

            // printPT(X);
            //  printPT(findIntersection(-1,0,1,0,0.5,-1,0.5,1));
            // printPT(ball_pos);
            // printf("x_dist: %lf, y_dist: %lf\n\n", X.x - ball_pos.x,  X.y - ball_pos.y);
            PT direction_vector = A - B;
            PT orthogonal_direction_vector = PT(-direction_vector.y, direction_vector.x, 0);
            orthogonal_direction_vector = orthogonal_direction_vector / sqrt(dot(orthogonal_direction_vector, orthogonal_direction_vector));
            ball_look = ball_look - orthogonal_direction_vector * 2 * dot(ball_look, orthogonal_direction_vector);
            ball_right = PT(ball_look.y, -ball_look.x, 0); // Orthogonal
        }
    }
    // printf("==============================Collision=================================\n");
}

PT next_ball_look, next_ball_right;
void predictCollision()
{
    // printf("==============================Collision=================================\n");
    // int n = 4;
    int i = getNearestWallManual(false);
    // for (int i = 0; i < n; i++)
    {
        PT A = vertex[i];
        PT B = vertex[(i + 1) % n];
        PT C = ball_pos;
        PT D = ball_pos + (ball_look * 100);
        // if (intersect(A, B, C, D))
        {
            PT direction_vector = A - B;
            PT orthogonal_direction_vector = PT(-direction_vector.y, direction_vector.x, 0);
            orthogonal_direction_vector = orthogonal_direction_vector / sqrt(dot(orthogonal_direction_vector, orthogonal_direction_vector));
            ball_look = ball_look - orthogonal_direction_vector * 2 * dot(ball_look, orthogonal_direction_vector);
            ball_right = PT(ball_look.y, -ball_look.x, 0); // Orthogonal
            // break;
        }
    }
}

// double getMinDistance()
// {
//     double dist = INT64_MAX;
//     for (int i = 0; i < n; i++)
//     {
//         PT tmp_look = ball_look;
//         PT A = vertex[i];
//         PT B = vertex[(i + 1) % n];
//         PT C = ball_pos;
//         PT D = ball_pos + (tmp_look * 100);
//         // if (intersect(A, B, C, D))
//         {
//             PT dir = B - A;
//             double theta = acos(dot2D(dir, tmp_look) / (det2D(dir) * det2D(tmp_look)));

//             // printf("Wall: %d, Theta: %lf\n", i, theta * 180 / PI);

//             PT I = findIntersectionInf(A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);

//             if (isnanf(I.x) || isnanf(I.y))
//             {
//                 continue;
//             }
//             // printPT(I);
//             // printPT(C);
//             // printPT(I);

//             PT tmp = I - C;

//             // printPT(ball_look);
//             // printPT(tmp);
//             double check_theta = acos(min(max(dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)), -1.0), 1.0));

//             // printf("Check: %lf %d\n", check_theta, std::fabs(check_theta) < 1e-9);
//             if (floor(check_theta) == 0)
//             {
//                 double distance = distancePointToPoint(C, I);
//                 double minus = ball_radius / sin(theta);
//                 dist = min(dist, distance - minus);

//                 printf("Dist: %lf, fULL: %lf, N: %lf, D: %lf, %lf\n", distance - minus, distance, dot2D(tmp, tmp_look), det2D(tmp) * det2D(tmp_look), dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)));
//             }
//         }
//     }
//     return dist;
// }

// int getCollidedWall()
// {
//     for (int i = 0; i < n; i++)
//     {
//         PT A = vertex[i];
//         PT B = vertex[(i + 1) % n];
//         PT C = ball_pos;
//         PT D = ball_pos + (ball_look * 100);
//         if (intersect(A, B, C, D))
//         {
//             return i;
//         }
//     }
//     return -1;
// }

double getMinDistanceManual(bool back)
{
    double dist = INT64_MAX;
    for (int i = 0; i < n; i++)
    {
        PT tmp_look = ball_look * (back ? -1 : 1);
        PT A = vertex[i];
        PT B = vertex[(i + 1) % n];
        PT C = ball_pos;
        PT D = ball_pos + (tmp_look * 100);
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
            // printPT(I);
            // printPT(C);
            // printPT(I);

            PT tmp = I - C;

            // printPT(ball_look);
            // printPT(tmp);
            double check_theta = acos(min(max(dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)), -1.0), 1.0));

            // printf("Check: %lf %d\n", check_theta, std::fabs(check_theta) < 1e-9);
            if (floor(check_theta) == 0)
            {
                double distance = distancePointToPoint(C, I);
                double minus = ball_radius / sin(theta);
                dist = min(dist, distance - minus);

                printf("Dist: %lf, fULL: %lf, N: %lf, D: %lf, %lf\n", distance - minus, distance, dot2D(tmp, tmp_look), det2D(tmp) * det2D(tmp_look), dot2D(tmp, tmp_look) / (det2D(tmp) * det2D(tmp_look)));
            }
        }
    }
    return dist;
}

bool checkCollision(bool back)
{
    double min_dist = getMinDistanceManual(back);
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
    angle = (angle + 1) % 360;
    counter += time;
    // printf("Time: %d, Counter: %d\n", time, counter);
    if (animate)
    {
        double d = min(getMinDistanceManual(false), 0.25);
        ball_pos = ball_pos + (ball_look * d);
        updateBall(-25 * d / 0.25, 8, 40);
        ball_rot_right -= 25 * d / 0.25;
        // if (checkCollision())
        // {
        //     // handleCollision();
        // }
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
                // handleCollision();
                // ball_look = next_ball_look;
                // ball_right = next_ball_right;
                predictCollision();
            }
            else
            {
                current_id++;
                id = current_id;
            }
            // predictCollision();
            double min_dist = getMinDistanceManual(false);
            time = min_dist / (det2D(ball_look) * 0.25 / 100);
            // printf("Dist: %lf\n", min_dist);

            // printf("Wall: %d\n, Look: ", wall);
            // printPT(ball_look);
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
    // if (key == 27) glutLeaveMainLoop();

    if (key == 'a')
        if (stacks > 0)
            stacks -= 1;
    if (key == 'd')
        stacks += 1;
    if (key == 'w')
        sectors += 1;
    if (key == 's')
        if (sectors > 0)
            sectors -= 1;
    if (key == 27)
        glutLeaveMainLoop();
    switch (key)
    {
    case '1':
        // rotate LEFT
        rotate3D(right, up, ROT_ANG);
        rotate3D(look, up, ROT_ANG);
        break;
    case '2':
        // rotate right
        rotate3D(right, up, -ROT_ANG);
        rotate3D(look, up, -ROT_ANG);
        break;
    case '3':
        // rotate UP
        rotate3D(up, right, ROT_ANG);
        rotate3D(look, right, ROT_ANG);
        break;
    case '4':
        // rotate DOWN
        rotate3D(up, right, -ROT_ANG);
        rotate3D(look, right, -ROT_ANG);
        break;
    case '5':
        // tilt CLKWISE
        rotate3D(right, look, ROT_ANG);
        rotate3D(up, look, ROT_ANG);
        break;
    case '6':
        // tilt COUNTER CLKWISE
        rotate3D(right, look, -ROT_ANG);
        rotate3D(up, look, -ROT_ANG);
        break;

    case 'w':
    case 'i':
        if (!animate)
        {
            ball_rot_right -= 10;
            ball_pos = ball_pos + (ball_look * min(0.1, getMinDistanceManual(false)));
            updateBall(-10, 8, 40);
            if (checkCollision(false))
            {
                handleCollision(false);
            }
        }
        break;

    case 's':
    case 'k':
        if (!animate)
        {
            ball_rot_right += 10;
            ball_pos = ball_pos - (ball_look * min(0.1, getMinDistanceManual(true)));
            updateBall(10, 8, 40);
            if (checkCollision(true))
            {
                handleCollision(true);
            }
        }
        break;

    case 'a':
    case 'j':
        dir_angle = (dir_angle + 10) % 360;
        ball_rot_z = (ball_rot_z + 10) % 360;

        rotate3D(ball_right, ball_up, ROT_ANG);
        rotate3D(ball_look, ball_up, ROT_ANG);
        predicted = false;
        if (animate)
            glutTimerFunc(0, collision, ++current_id);
        // printf("%lf %lf\n",atan(ball_look.y / ball_look.x), fmod((atan2(ball_look.y, ball_look.x)* 180 / PI)+360.0,360.0));
        break;

    case 'd':
    case 'l':
        dir_angle = (360 + dir_angle - 10) % 360;
        ball_rot_z = (360 + ball_rot_z - 10) % 360;

        rotate3D(ball_right, ball_up, -ROT_ANG);
        rotate3D(ball_look, ball_up, -ROT_ANG);
        predicted = false;
        // getMinDistance();
        if (animate)
            glutTimerFunc(0, collision, ++current_id);
        // printf("%lf %lf\n",atan(ball_look.y / ball_look.x), fmod((atan2(ball_look.y, ball_look.x)* 180 / PI)+360.0,360.0));
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
        ball_pos.x = 0;
        ball_pos.y = 0;
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
        ball_pos.x = 0;
        ball_pos.y = 0;
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

    // ball_pos = minX(maxX(ball_pos, PT(-5.5,ball_pos.y,ball_pos.z)), PT(5.5,ball_pos.y,ball_pos.z));
    // ball_pos = minY(maxY(ball_pos, PT(ball_pos.x,-5.5,ball_pos.z)), PT(ball_pos.x,5.5,ball_pos.z));

    glutPostRedisplay();
}

void specialkey(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        pos = pos + (look * 0.1);
        break;
    case GLUT_KEY_DOWN:
        pos = pos - (look * 0.1);
        break;

    case GLUT_KEY_RIGHT:
        pos = pos + (right * 0.1);
        break;

    case GLUT_KEY_LEFT:
        pos = pos - (right * 0.1);
        break;

    case GLUT_KEY_PAGE_UP:
        pos = pos + (up * 0.1);
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos = pos - (up * 0.1);
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
        pos.x -= 0.1;
        pos.y -= 0.1;
        pos.z -= 0.1;
    }
    else if (button == 4)
    {
        pos.x += 0.1;
        pos.y += 0.1;
        pos.z += 0.1;
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