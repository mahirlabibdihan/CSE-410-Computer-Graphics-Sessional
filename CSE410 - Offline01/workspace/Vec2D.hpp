#ifndef _VEC_2_H
#define _VEC_2_H
#include "Vec3D.hpp"
class Vec2D : public Vec3D
{
public:
    Vec2D();
    Vec2D(double x, double y);
    Vec2D(const Vec2D &v);

    Vec2D operator+(Vec2D b);
    Vec2D operator+=(Vec2D b);
    Vec2D operator-(Vec2D b);
    Vec2D operator-();
    Vec2D operator-=(Vec2D b);
    Vec2D operator*(double b);
    Vec2D operator*=(double b);
    Vec2D operator/(double b);
    Vec2D operator/=(double b);
    Vec2D operator/(Vec2D b);
    Vec2D operator/=(Vec2D b);

    double magnitude();
    Vec2D normalize();
    Vec2D perp();
    Vec2D rotate(double radian);
    Vec2D reflect(Vec2D r);
    double angle(Vec2D b);

    bool operator==(Vec2D b);
    double dot(Vec2D b);
};

#endif