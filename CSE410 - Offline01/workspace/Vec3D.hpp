#ifndef _VEC_3_H
#define _VEC_3_H
#include <iostream>
using namespace std;
class Vec3D
{
public:
    double x, y, z;
    Vec3D();
    Vec3D(double x, double y, double z);
    Vec3D(const Vec3D &p);
    Vec3D operator+(Vec3D b);
    Vec3D operator+=(Vec3D b);
    Vec3D operator-(Vec3D b);
    Vec3D operator-=(Vec3D b);
    Vec3D operator*(double b);
    Vec3D operator*=(double b);
    Vec3D operator/(double b);
    Vec3D operator/=(double b);
    Vec3D operator/(Vec3D b);
    Vec3D operator/=(Vec3D b);
    Vec3D operator*(Vec3D b);
    Vec3D operator*=(Vec3D b);
    double angle(Vec3D b);
    Vec3D perpRotate(Vec3D axis, double radian);
    Vec3D rotate(Vec3D axis, double radian);
    double magnitude();
    Vec3D normalize();
    double dot(Vec3D b);

    friend ostream &operator<<(ostream &os, const Vec3D &dt);
};

ostream &operator<<(ostream &os, const Vec3D &v);
#endif