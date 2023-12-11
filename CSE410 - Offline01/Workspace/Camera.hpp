#include "Point.hpp"
#include <math.h>
#include "GLM.hpp"
class Camera
{
public:
    PT pos, up, right, look;
    Camera()
    {
        init();
    }
    void init()
    {
        pos = PT(10, 10, 10);
        up = PT(-1 / sqrt(3), -1 / sqrt(3), 1 / sqrt(3));
        right = PT(-1 / sqrt(2), 1 / sqrt(2), 0);
        look = PT(-1 / sqrt(3), -1 / sqrt(3), -1 / sqrt(3));
    }
    void setCamera()
    {
        gluLookAt(pos.x, pos.y, pos.z,
                  pos.x + look.x, pos.y + look.y, pos.z + look.z,
                  up.x, up.y, up.z);
    }
    void rotateAroundUp(double radian)
    {
        rotate3D(right, up, radian);
        rotate3D(look, up, radian);
    }
    void rotateLeft()
    {
        rotateAroundUp(radians(1));
    }
    void rotateRight()
    {
        rotateAroundUp(-radians(1));
    }
    void rotateAroundRight(double radian)
    {
        rotate3D(up, right, radian);
        rotate3D(look, right, radian);
    }
    void rotateUp()
    {
        rotateAroundRight(radians(1));
    }
    void rotateDown()
    {
        rotateAroundRight(-radians(1));
    }
    void rotateAroundLook(double radian)
    {
        rotate3D(right, look, radian);
        rotate3D(up, look, radian);
    }
    void tiltClock()
    {
        rotateAroundLook(radians(1));
    }
    void tiltCounterClock()
    {
        rotateAroundLook(-radians(1));
    }
    void moveForward()
    {
        pos = pos + (look * 0.1);
    }
    void moveBackward()
    {
        pos = pos - (look * 0.1);
    }
    void moveLeft()
    {
        pos = pos - (right * 0.1);
    }
    void moveRight()
    {
        pos = pos + (right * 0.1);
    }
    void moveUp()
    {
        pos = pos + (up * 0.1);
    }
    void moveDown()
    {
        pos = pos - (up * 0.1);
    }
    void moveAroundRef(int dir)
    {
        PT center = findReference(pos, look);
        double old_dist = distancePointToPoint(center, pos);
        PT old_pos = pos;
        pos.z = pos.z + dir * .5;
        PT new_pos = pos;
        double new_dist = distancePointToPoint(center, pos);
        double ang = angleBetweenVectors(old_pos - center, new_pos - center);
        PT axis = findPerpendicularVector(look);
        rotate3D(look, axis, dir * ang);
        up = right * look;
        right = look * up;
    }
    void moveUpRef()
    {
        moveAroundRef(1);
    }
    void moveDownRef()
    {
        moveAroundRef(-1);
    }
};