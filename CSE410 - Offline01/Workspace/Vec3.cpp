#include "Vec3.hpp"
#include "Headers.hpp"
Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {}
Vec3::Vec3(const Vec3 &p) : x(p.x), y(p.y), z(p.z) {}
Vec3 Vec3::operator+(Vec3 b) { return Vec3(x + b.x, y + b.y, z + b.z); }
Vec3 Vec3::operator+=(Vec3 b) { return Vec3(x + b.x, y + b.y, z + b.z); }
Vec3 Vec3::operator-(Vec3 b) { return Vec3(x - b.x, y - b.y, z - b.z); }
Vec3 Vec3::operator-=(Vec3 b) { return Vec3(x - b.x, y - b.y, z - b.z); }
Vec3 Vec3::operator*(double b) { return Vec3(x * b, y * b, z * b); }
Vec3 Vec3::operator*=(double b) { return Vec3(x * b, y * b, z * b); }
Vec3 Vec3::operator/(double b) { return Vec3(x / b, y / b, z / b); }
Vec3 Vec3::operator/=(double b) { return Vec3(x / b, y / b, z / b); }
Vec3 Vec3::operator/(Vec3 b) { return Vec3(x / b.x, y / b.y, z / b.y); }
Vec3 Vec3::operator/=(Vec3 b) { return Vec3(x / b.x, y / b.y, z / b.y); }
Vec3 Vec3::operator*(Vec3 b) { return Vec3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x); }
Vec3 Vec3::operator*=(Vec3 b) { return Vec3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x); }

double dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
double dot2D(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y; }
void printPT(Vec3 a) { printf("(%lf,%lf,%lf)\n", a.x, a.y, a.z); }
