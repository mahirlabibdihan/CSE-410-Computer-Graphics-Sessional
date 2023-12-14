#ifndef _GLM_H
#define _GLM_H
#include "Headers.hpp"

double radians(double degrees);
double degrees(double radians);

Vec3D findReference(Vec3D point, Vec3D normalizedVector);
double distancePointToPoint2D(Vec2D a, Vec2D b);
double distancePointToPoint3D(Vec3D a, Vec3D b);

Vec3D findPerpendicularVector(Vec3D normalizedVector);

Vec2D getIntersectionPoint(Vec2D a_point, Vec2D a_dir, Vec2D b_point, Vec2D b_dir);
double distancePointToLine(double x0, double y0, double x1, double y1, double x2, double y2);
Vec2D findIntersectionInf(double x1, double y1, double x2, double y2,
                          double x3, double y3, double x4, double y4);

bool haveSameDirection(Vec2D a, Vec2D b);

bool doRayIntersectLine(Vec2D ray_p, Vec2D ray_d, Vec2D line_p, Vec2D line_d);
bool doIntersect(Vec2D a_dir, Vec2D b_dir);

#endif