#include <cmath>
#include <iostream>

struct Point
{
    double x, y;
};

// Overload the << operator for Point
std::ostream &operator<<(std::ostream &os, const Point &point)
{
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}
double dist(const Point &p, const Point &q)
{
    return sqrt((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y));
}

double angle(const Point &p, const Point &ref)
{
    if (p.x == ref.x && p.y == ref.y) 
        return 0.0;
    return std::atan2(p.y - ref.y, p.x - ref.x);
}

bool compare_angles(const Point &p, const Point &q, const Point &ref)
{
    return angle(p, ref) < angle(q, ref);
}
bool are_colinear(const Point &p, const Point &q, const Point &r)
{
    double tolerance = 1e-9; // Adjust tolerance as needed
    return (std::abs(angle(p, q) - angle(p, r)) < tolerance) || std::abs(angle(p, q) + angle(p, r) - M_PI)<tolerance ;
}

bool is_convex(const Point &p, const Point &q, const Point &r)
{
    return (q.x - p.x) * (r.y - p.y) - (r.x - p.x) * (q.y - p.y) > 0;
}