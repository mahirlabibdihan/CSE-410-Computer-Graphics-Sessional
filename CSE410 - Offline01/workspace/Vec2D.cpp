#include "Vec2D.hpp"
#include "Headers.hpp"
Vec2D::Vec2D() : Vec3D() {}
Vec2D::Vec2D(double x, double y) : Vec3D(x, y, 0) {}
Vec2D::Vec2D(const Vec2D &p) : Vec3D(p.x, p.y, 0) {}

Vec2D Vec2D::operator+(Vec2D b) { return Vec2D(x + b.x, y + b.y); }
Vec2D Vec2D::operator+=(Vec2D b)
{
    *this = *this + b;
    return *this;
}
Vec2D Vec2D::operator-(Vec2D b) { return Vec2D(x - b.x, y - b.y); }
Vec2D Vec2D::operator-()
{
    return *this * (-1);
}
Vec2D Vec2D::operator-=(Vec2D b)
{
    this->x -= b.x;
    this->y -= b.y;
    this->z -= b.z;
    return *this;
}
Vec2D Vec2D::operator*(double b) { return Vec2D(x * b, y * b); }

Vec2D Vec2D::operator/(double b) { return Vec2D(x / b, y / b); }
Vec2D Vec2D::operator/=(Vec2D b)
{
    this->x /= b.x;
    this->y /= b.y;
    this->z /= b.z;
    return *this;
}
Vec2D Vec2D::operator/(Vec2D b) { return Vec2D(x / b.x, y / b.y); }
double Vec2D::dot(Vec2D b)
{
    return x * b.x + y * b.y;
}

bool Vec2D::operator==(Vec2D b)
{
    return x == b.x && y == b.y;
}

double Vec2D::magnitude()
{
    return sqrt(this->dot(*this));
}
Vec2D Vec2D::normalize()
{
    return *this / magnitude();
}
Vec2D Vec2D::perp()
{
    return Vec2D(-y, x);
}
Vec2D Vec2D::rotate(double radian)
{
    Vec2D v_perp = this->perp();
    return *this * cos(radian) + v_perp * sin(radian);
}

Vec2D Vec2D::reflect(Vec2D r)
{
    Vec2D n = r.perp().normalize();
    return *this - n * 2 * this->dot(n);
}
double Vec2D::angle(Vec2D b)
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