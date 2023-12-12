#ifndef _CAMERA_H
#define _CAMERA_H
#include "Vec3.hpp"

class Camera
{
public:
    Vec3 pos;
    Vec3 up;
    Vec3 right;
    Vec3 look;
    Camera();
    void init();
    void setCamera();
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
};
#endif