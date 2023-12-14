#include "Vec3D.hpp"
#include "Headers.hpp"
Vec3D::Vec3D() : x(0), y(0), z(0) {}
Vec3D::Vec3D(double x, double y, double z) : x(x), y(y), z(z) {}
Vec3D::Vec3D(const Vec3D &p) : x(p.x), y(p.y), z(p.z) {}
Vec3D Vec3D::operator+(Vec3D b) { return Vec3D(x + b.x, y + b.y, z + b.z); }
Vec3D Vec3D::operator+=(Vec3D b)
{
    *this = *this + b;
    return *this;
}
Vec3D Vec3D::operator-(Vec3D b) { return Vec3D(x - b.x, y - b.y, z - b.z); }
Vec3D Vec3D::operator-=(Vec3D b)
{
    this->x -= b.x;
    this->y -= b.y;
    this->z -= b.z;
    return *this;
}
Vec3D Vec3D::operator*(double b) { return Vec3D(x * b, y * b, z * b); }
Vec3D Vec3D::operator*=(Vec3D b)
{
    this->x *= b.x;
    this->y *= b.y;
    this->z *= b.z;
    return *this;
}
Vec3D Vec3D::operator/(double b) { return Vec3D(x / b, y / b, z / b); }
Vec3D Vec3D::operator/=(Vec3D b)
{
    this->x /= b.x;
    this->y /= b.y;
    this->z /= b.z;
    return *this;
}
Vec3D Vec3D::operator/(Vec3D b) { return Vec3D(x / b.x, y / b.y, z / b.y); }
Vec3D Vec3D::operator*(Vec3D b) { return Vec3D(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x); }

double dot(Vec3D a, Vec3D b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
double dot2D(Vec3D a, Vec3D b) { return a.x * b.x + a.y * b.y; }
void printPT(Vec3D a) { printf("(%lf,%lf,%lf)\n", a.x, a.y, a.z); }
double Vec3D::dot(Vec3D b)
{
    return x * b.x + y * b.y + z * b.z;
}
Vec3D Vec3D::perpRotate(Vec3D axis, double radian) // Counter Clock
{
    Vec3D l = *this;
    Vec3D r = axis.normalize();
    return l * cos(radian) + (r * l) * sin(radian);
}
Vec3D Vec3D::rotate(Vec3D axis, double radian) // Counter Clock
{
    return *this * cos(radian) + axis * (1 - cos(radian)) * this->dot(axis) + (axis * *this) * sin(radian);
}

double Vec3D::magnitude()
{
    return sqrt(this->dot(*this));
}
Vec3D Vec3D::normalize()
{
    return *this / this->magnitude();
}
double Vec3D::angle(Vec3D b)
{
    double magA = this->magnitude();
    double magB = b.magnitude();

    if (magA == 0.0 || magB == 0.0)
    {
        return NAN;
    }

    double cosTheta = this->dot(b) / (magA * magB);
    return acos(max(-1.0, min(1.0, cosTheta)));
}

ostream &operator<<(ostream &os, const Vec3D &v)
{
    os << '(' << v.x << ',' << v.y << ',' << v.z << ')';
    return os;
}