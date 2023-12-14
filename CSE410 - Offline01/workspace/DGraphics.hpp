#ifndef _D_GRAPHICS_H
#define _D_GRAPHICS_H
#include "OpenGL.hpp"
class DGraphics
{
    static GLint WindowHeight, WindowWidth;

public:
    static void keyHandler(unsigned char, int, int);
    static void specialKeyHandler(int, int, int);
    static void keyboard(unsigned char);
    static void specialKeyboard(int);

    static void displayHandler();
    static void display();

    static void resize(int, int);

    static void mouseClickHandler(int, int, int, int);
    static void mouseScroll(int);

    static void initialize(int, int, char *);
    static void init();

    static void drawCheckerboard();
    static void drawSquare(double a);
    static void drawAxis();
    static void drawCube();
    static void drawCylinder(float radius, float height);
    static void drawCone(float radius, float height);
    static void drawArrow(double length);
};
#endif