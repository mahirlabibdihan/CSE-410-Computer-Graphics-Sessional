#include "GLM.hpp"
#include "Vec2D.hpp"
#include "Headers.hpp"

bool doRayIntersectLine(Vec2D ray_p, Vec2D ray_d, Vec2D line_p, Vec2D line_d)
{
    // double tRay = ((A.y - C.y) * wall_dir.x - (A.x - C.x) * wall_dir.y) /
    //               (ball_look.x * wall_dir.y - ball_look.y * wall_dir.x);

    // tRay <= 0
    if (abs(ray_d.x * line_d.y - ray_d.y * line_d.x) > 1e-9)
    {
        Vec2D I = getIntersectionPoint(ray_p, ray_d, line_p, line_d);
        return haveSameDirection(I - ray_p, ray_d);
    }
    return false;
}

bool doIntersect(Vec2D a_dir, Vec2D b_dir)
{
    return abs(a_dir.x * b_dir.y - a_dir.y * b_dir.x) > 1e-9;
}
bool haveSameDirection(Vec2D v1, Vec2D v2)
{
    double crossProduct = v1.x * v2.y - v1.y * v2.x;
    double dotProduct = v1.x * v2.x + v1.y * v2.y;

    return abs(crossProduct) < 1e-9 && dotProduct > 0;
}
// bool doIntersect(Line3D a, Line3D b)
// {
//     return (a.point - b.point).dot(a.dir * b.dir) == 0;
// }

Vec2D findIntersectionInf(double x1, double y1, double x2, double y2,
                          double x3, double y3, double x4, double y4)
{
    Vec2D intersection;

    double det = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));

    if (std::abs(det) < 1e-9)
    {
        intersection.x = NAN;
        intersection.y = NAN;
    }
    else
    {
        intersection.x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / det;
        intersection.y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / det;
    }

    return intersection;
}

Vec2D getIntersectionPoint(Vec2D a_point, Vec2D a_dir, Vec2D b_point, Vec2D b_dir)
{
    if (!doIntersect(a_dir, b_dir))
        return {NAN, NAN};

    // cout << "Not parallel" << endl;
    double x1 = a_point.x, y1 = a_point.y;
    double x2 = b_point.x, y2 = b_point.y;
    double dx1 = a_dir.x, dy1 = a_dir.y;
    double dx2 = b_dir.x, dy2 = b_dir.y;
    double det = dx1 * dy2 - dy1 * dx2;

    double t1 = ((x2 - x1) * dy2 + (y1 - y2) * dx2) / det;

    double intersectX = x1 + t1 * dx1;
    double intersectY = y1 + t1 * dy1;
    // cout << intersectX << " " << intersectY << endl;
    return {intersectX, intersectY};
}

double radians(double degrees)
{
    return degrees * M_PI / 180;
}
double degrees(double radians)
{
    return radians * 180 / M_PI;
}

Vec3D findReference(Vec3D p, Vec3D v)
{
    // The equation of the XY plane is z = 0
    // To find the intersection point, set z to 0 in the equation of the line
    // 0.0 = p.z + t * v.y;
    double t = -p.z / v.z;

    // Calculate the intersection point
    Vec3D intersection;
    intersection.x = p.x + t * v.x;
    intersection.y = p.y + t * v.y;
    intersection.z = 0.0; // Intersection point lies on the XY plane

    return intersection;
}
double distancePointToPoint2D(Vec2D a, Vec2D b)
{
    return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}
double distancePointToPoint3D(Vec3D a, Vec3D b)
{
    return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z));
}

Vec3D findPerpendicularVector(Vec3D normalizedVector) // In XY plane
{
    // Choose any vector in the XY plane with the same z-component
    // For example, (1, 0, 0) or (0, 1, 0)
    Vec3D perpendicularVector(1.0, 0.0, 0.0);

    // Adjust the vector to make it perpendicular to the given normalized vector
    // normalizedVector.x / normalizedVector.y = perpendicularVector.x / perpendicularVector.y

    double mag = sqrt(normalizedVector.x * normalizedVector.x + normalizedVector.y * normalizedVector.y);
    perpendicularVector = perpendicularVector - normalizedVector * normalizedVector.dot(perpendicularVector);
    return Vec3D(-normalizedVector.y / mag, normalizedVector.x / mag, 0.0);
}

double distancePointToLine(double x0, double y0, double x1, double y1, double x2, double y2)
{
    double numerator = std::abs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1);
    double denominator = std::sqrt(std::pow(y2 - y1, 2) + std::pow(x2 - x1, 2));
    return numerator / denominator;
}
