
#ifndef _POINT_H
#define _POINT_H
struct PT
{
    PT() {}
    double x, y, z;

    PT(double x, double y, double z) : x(x), y(y), z(z) {}
    PT(const PT &p) : x(p.x), y(p.y), z(p.z) {}

    // arithemtic operations
    PT operator+(PT b) { return PT(x + b.x, y + b.y, z + b.z); }
    PT operator-(PT b) { return PT(x - b.x, y - b.y, z - b.z); }
    PT operator*(double b) { return PT(x * b, y * b, z * b); }
    PT operator/(double b) { return PT(x / b, y / b, z / b); }
    PT operator/(PT b) { return PT(x / b.x, y / b.y, z / b.y); }
    PT operator*(PT b) { return PT(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x); }
};

PT minX(PT p, PT q)
{
    return p.x < q.x ? p : q;
}
PT maxX(PT p, PT q)
{
    return p.x > q.x ? p : q;
}
PT minY(PT p, PT q)
{
    return p.y < q.y ? p : q;
}
PT maxY(PT p, PT q)
{
    return p.y > q.y ? p : q;
}

double dot(PT a, PT b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
double dot2D(PT a, PT b) { return a.x * b.x + a.y * b.y; }
#endif