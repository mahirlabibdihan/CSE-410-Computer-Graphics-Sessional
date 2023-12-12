#include "Headers.hpp"

class Camera
{
public:
    Vec3 pos, up, right, look;
    Camera()
    {
        init();
    }
    void init()
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
    void setCamera()
    {
        gluLookAt(
            pos.x, pos.y, pos.z,
            // 11, 11, 11,
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
    void moveUpRef()
    {
        moveAroundRef(1);
    }
    void moveDownRef()
    {
        moveAroundRef(-1);
    }
    void draw()
    {
        // glPushMatrix();
        // // Direction arrow
        // glPushMatrix();
        // glTranslatef(pos.x, pos.y, pos.z);
        // // glRotatef(fmod((atan2(look.y, look.x) * 180 / M_PI) + 360.0, 360.0), right.x, right.y, right.z);
        // // glRotatef(90, 0, 1, 0);
        // glColor3f(0.0, 0.0, 1.0); // Red color for the cylinder
        // // drawArrow(1.0);
        // glBegin(GL_LINES);
        // glVertex3f(0, 0, 0);
        // glVertex3f(look.x, look.y, look.z);
        // glEnd();
        // glPopMatrix();

        // // glPushMatrix();
        // // glTranslatef(pos.x, pos.y, pos.z);
        // // glRotatef(fmod((atan2(look.y, look.x) * 180 / M_PI) + 360.0, 360.0), up.x, up.y, up.z);
        // // glRotatef(90, 0, 1, 0);
        // // glColor3f(0.0, 0.0, 1.0); // Red color for the cylinder
        // // drawArrow(1.0);
        // glPopMatrix();
    }
};