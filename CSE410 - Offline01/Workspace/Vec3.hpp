#ifndef _VEC_3_H
#define _VEC_3_H
class Vec3
{
public:
    double x, y, z;
    Vec3();
    Vec3(double x, double y, double z);
    Vec3(const Vec3 &p);
    Vec3 operator+(Vec3 b);
    Vec3 operator+=(Vec3 b);
    Vec3 operator-(Vec3 b);
    Vec3 operator-=(Vec3 b);
    Vec3 operator*(double b);
    Vec3 operator*=(double b);
    Vec3 operator/(double b);
    Vec3 operator/=(double b);
    Vec3 operator/(Vec3 b);
    Vec3 operator/=(Vec3 b);
    Vec3 operator*(Vec3 b);
    Vec3 operator*=(Vec3 b);
};

double dot(Vec3 a, Vec3 b);
double dot2D(Vec3 a, Vec3 b);
#endif