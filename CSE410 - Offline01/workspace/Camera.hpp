#ifndef _CAMERA_H
#define _CAMERA_H
#include "Vec3D.hpp"

class Camera
{
    Vec3D pos, up, right, look;
    double focal_length;

public:
    Camera();
    void init(Vec3D p, Vec3D u, Vec3D r, Vec3D l);
    void set();
    void rotateAroundUp(double radian);
    void rotateLeft();
    void rotateRight();
    void rotateAroundRight(double radian);
    void rotateUp();
    void rotateDown();
    void rotateAroundLook(double radian);
    void tiltClock();
    void tiltCounterClock();
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void moveAroundRef(int dir);
    void moveUpRef();
    void moveDownRef();
    void drawReference();
};
#endif