#include "GLM.hpp"

void rotate3D(Vec3 &vec, Vec3 &axis, double ang)
{
    vec = vec * cos(ang) + axis * (1 - cos(ang)) * dot(axis, vec) + (axis * vec) * sin(ang);
}
double radians(double degrees)
{
    return degrees * M_PI / 180;
}
double degrees(double radians)
{
    return radians * 180 / M_PI;
}
double det2D(Vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

double det(Vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
Vec3 findReference(Vec3 point, Vec3 normalizedVector)
{
    // The equation of the XY plane is z = 0
    // To find the intersection point, set z to 0 in the equation of the line
    double t = -point.z / normalizedVector.z;

    // Calculate the intersection point
    Vec3 intersectionPoint;
    intersectionPoint.x = point.x + t * normalizedVector.x;
    intersectionPoint.y = point.y + t * normalizedVector.y;
    intersectionPoint.z = 0.0; // Intersection point lies on the XY plane

    return intersectionPoint;
}
double distancePointToPoint(Vec3 a, Vec3 b)
{
    return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}
double angleBetweenVectors(Vec3 A, Vec3 B)
{
    // if (A.size() != B.size())
    // {
    //     std::cerr << "Vectors must have the same dimension." << std::endl;
    //     return 0.0; // Or some error value
    // }

    double dot3D = dot(A, B);
    double magA = det(A);
    double magB = det(B);

    // Ensure denominators are not zero
    if (magA == 0.0 || magB == 0.0)
    {
        // std::cerr << "Magnitudes must be non-zero." << std::endl;
        return 0.0; // Or some error value
    }

    double cosTheta = dot3D / (magA * magB);
    return acos(max(-1.0, min(1.0, cosTheta))); // Ensure cosTheta is within [-1, 1]
}

Vec3 findPerpendicularVector(Vec3 normalizedVector)
{
    // Choose any vector in the XY plane with the same z-component
    // For example, (1, 0, 0) or (0, 1, 0)
    Vec3 perpendicularVector(1.0, 0.0, 0.0);

    // Adjust the vector to make it perpendicular to the given normalized vector
    // normalizedVector.x / normalizedVector.y = perpendicularVector.x / perpendicularVector.y

    double mag = sqrt(normalizedVector.x * normalizedVector.x + normalizedVector.y * normalizedVector.y);
    perpendicularVector = perpendicularVector - normalizedVector * dot(normalizedVector, perpendicularVector);
    return Vec3(-normalizedVector.y / mag, normalizedVector.x / mag, 0.0);
}

double distancePointToLine(double x0, double y0, double x1, double y1, double x2, double y2)
{
    double numerator = std::abs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1);
    double denominator = std::sqrt(std::pow(y2 - y1, 2) + std::pow(x2 - x1, 2));
    return numerator / denominator;
}

Vec3 findIntersectionInf(double x1, double y1, double x2, double y2,
                         double x3, double y3, double x4, double y4)
{
    Vec3 intersection;

    double det = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));

    if (std::abs(det) < 1e-9)
    {
        // Lines are parallel or nearly parallel
        // std::cout << "Lines are parallel. No unique intersection point." << std::endl;
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