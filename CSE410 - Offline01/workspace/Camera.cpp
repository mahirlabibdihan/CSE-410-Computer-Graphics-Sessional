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
    Vec3D center = findReference(pos, look);
    double old_dist = distancePointToPoint3D(center, pos);
    Vec3D old_pos = pos;

    if (pos.z == -dir * .1)
    {
        pos.z = pos.z + dir * .11;
    }
    else
    {
        pos.z = pos.z + dir * .1;
    }

    Vec3D new_pos = pos;
    double new_dist = distancePointToPoint3D(center, pos);
    double ang = (old_pos - center).angle(new_pos - center);

    // cout << center << endl;
    // cout << new_dist << endl;
    Vec3D axis = findPerpendicularVector(look); // On XY plane

    look = look.perpRotate(axis, dir * ang);
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