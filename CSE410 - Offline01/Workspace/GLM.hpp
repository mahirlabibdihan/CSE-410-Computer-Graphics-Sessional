#ifndef _GLM_H
#define _GLM_H
#include "Vec3.hpp"
#include <cmath>
using namespace std;

void rotate3D(Vec3 &vec, Vec3 &axis, double ang);
double radians(double degrees);
double degrees(double radians);
double det2D(Vec3 v);
double det(Vec3 v);
Vec3 findReference(Vec3 point, Vec3 normalizedVector);
double distancePointToPoint(Vec3 a, Vec3 b);
double angleBetweenVectors(Vec3 A, Vec3 B);

Vec3 findPerpendicularVector(Vec3 normalizedVector);

double distancePointToLine(double x0, double y0, double x1, double y1, double x2, double y2);
Vec3 findIntersectionInf(double x1, double y1, double x2, double y2,
                         double x3, double y3, double x4, double y4);
#endif