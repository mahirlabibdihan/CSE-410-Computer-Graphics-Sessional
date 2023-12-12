#include "Camera.hpp"
#include "Headers.hpp"
Camera::Camera()
{
    init();
}
void Camera::init()
{
    pos = Vec3(10, 10, 10);
    up = Vec3(-1 / sqrt(3), -1 / sqrt(3), 1 / sqrt(3));
    right = Vec3(-1 / sqrt(2), 1 / sqrt(2), 0);
    look = Vec3(-1 / sqrt(3), -1 / sqrt(3), -1 / sqrt(3));
    // pos = Vec3(10, 0, 10);
    // look = Vec3(-1 / sqrt(2), 0, -1 / sqrt(2));
    // right = Vec3(0, 1, 0);
    // up = Vec3(-1 / sqrt(2), 0, 1 / sqrt(2));
}
void Camera::setCamera()
{
    gluLookAt(
        pos.x, pos.y, pos.z,
        // 11, 11, 11,
        pos.x + look.x, pos.y + look.y, pos.z + look.z,
        up.x, up.y, up.z);
}
void Camera::rotateAroundUp(double radian)
{
    rotate3D(right, up, radian);
    rotate3D(look, up, radian);
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
    rotate3D(up, right, radian);
    rotate3D(look, right, radian);
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
    rotate3D(right, look, radian);
    rotate3D(up, look, radian);
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
    pos = pos + (look * 0.1);
}
void Camera::moveBackward()
{
    pos = pos - (look * 0.1);
}
void Camera::moveLeft()
{
    pos = pos - (right * 0.1);
}
void Camera::moveRight()
{
    pos = pos + (right * 0.1);
}
void Camera::moveUp()
{
    pos = pos + (up * 0.1);
}
void Camera::moveDown()
{
    pos = pos - (up * 0.1);
}
void Camera::moveAroundRef(int dir)
{
    Vec3 center = findReference(pos, look);
    double old_dist = distancePointToPoint(center, pos);
    Vec3 old_pos = pos;
    pos.z = pos.z + dir * .5;
    Vec3 new_pos = pos;
    double new_dist = distancePointToPoint(center, pos);
    double ang = angleBetweenVectors(old_pos - center, new_pos - center);
    Vec3 axis = findPerpendicularVector(look);
    rotate3D(look, axis, dir * ang);
    up = right * look;
    right = look * up;
}
void Camera::moveUpRef()
{
    moveAroundRef(1);
}
void Camera::moveDownRef()
{
    moveAroundRef(-1);
}