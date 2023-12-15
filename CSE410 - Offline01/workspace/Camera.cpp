#include "Camera.hpp"
#include "Headers.hpp"
Camera::Camera()
{
    init(Vec3D(7, 7, 7), Vec3D(-1 / sqrt(3), -1 / sqrt(3), 1 / sqrt(3)), Vec3D(-1 / sqrt(2), 1 / sqrt(2), 0), Vec3D(-1 / sqrt(3), -1 / sqrt(3), -1 / sqrt(3)));
}
void Camera::init(Vec3D p, Vec3D u, Vec3D r, Vec3D l)
{
    pos = p;
    up = u;
    right = r;
    look = l;
    focal_length = pos.magnitude();

    // pos = Vec3D(10, 0, 10);
    // look = Vec3D(-1 / sqrt(2), 0, -1 / sqrt(2));
    // right = Vec3D(0, 1, 0);
    // up = Vec3D(-1 / sqrt(2), 0, 1 / sqrt(2));
}
void Camera::set()
{
    gluLookAt(
        pos.x, pos.y, pos.z,
        // 11, 11, 11,fd
        pos.x + look.x, pos.y + look.y, pos.z + look.z,
        up.x, up.y, up.z);
}
void Camera::rotateAroundUp(double radian)
{
    right = right.perpRotate(up, radian);
    look = look.perpRotate(up, radian);
}
void Camera::rotateLeft()
{
    rotateAroundUp(radians(1));
}
void Camera::rotateRight()
{
    rotateAroundUp(-radians(1));
}
void Camera::rotateAroundRight(double radian)
{
    up = up.perpRotate(right, radian);
    look = look.perpRotate(right, radian);
}
void Camera::rotateUp()
{
    rotateAroundRight(radians(1));
}
void Camera::rotateDown()
{
    rotateAroundRight(-radians(1));
}
void Camera::rotateAroundLook(double radian)
{
    right = right.perpRotate(look, radian);
    up = up.perpRotate(look, radian);
}
void Camera::tiltClock()
{
    rotateAroundLook(radians(1));
}
void Camera::tiltCounterClock()
{
    rotateAroundLook(-radians(1));
}
void Camera::moveForward()
{
    pos = pos + (look * 0.5);
}
void Camera::moveBackward()
{
    pos = pos - (look * 0.5);
}
void Camera::moveLeft()
{
    pos = pos - (right * 0.5);
}
void Camera::moveRight()
{
    pos = pos + (right * 0.5);
}
void Camera::moveUp()
{
    pos = pos + (up * 0.5);
}
void Camera::moveDown()
{
    pos = pos - (up * 0.5);
}
void Camera::moveAroundRef(int dir)
{
    // double focal_length = pos.magnitude();

    Vec3D C = pos + look * focal_length;

    Vec3D A = pos;
    pos.z += dir * .5;

    Vec3D B = pos;
    Vec3D normalVector = (A - C) * (B - C);

    double ang = (A - C).angle(B - C);
    look = look.perpRotate(normalVector, ang);
    up = right * look;
    right = look * up;

    focal_length = distancePointToPoint3D(pos, C);
}
void Camera::moveUpRef()
{
    moveAroundRef(1);
}
void Camera::moveDownRef()
{
    moveAroundRef(-1);
}

void Camera::drawReference()
{
    // double focal_length = pos.magnitude();
    Vec3D C = pos + look * focal_length;

    glPushMatrix();
    glTranslatef(C.x, C.y, C.z);
    glScalef(0.5, 0.5, 0.5);
    DGraphics::drawAxis();
    glPopMatrix();
}
