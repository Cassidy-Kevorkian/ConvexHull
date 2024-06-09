#include "points.h"
#include <cmath>
#include <cstdint>
#include <iostream>

std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << point.x << " " << point.y;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Line &line) {
    if (line.a == 0 && line.c == 0)
        os << line.b << "y = 0";

    else if (line.a == 0)
        os << line.b << "y + " << line.c << " = 0";

    else if (line.b == 0 && line.c == 0)
        os << line.a << "x = 0";

    else if (line.b == 0)
        os << line.a << "x + " << line.c << " = 0";

    else if (line.c == 0)
        os << line.a << "x + " << line.b << "y = 0";

    else
        os << line.a << "x + " << line.b << "y + " << line.c << "= 0";

    return os;
}

bool Point::operator==(const Point &other) const {
    return x == other.x && y == other.y;
}

Point Point::operator+(const Point &other) const {
    return Point(x + other.x, y + other.y);
}

Point Point::operator-(const Point &other) const {
    return Point(x - other.x, y - other.y);
}

bool Point::operator<(const Point &other) const {
    if (this->x < other.x)
        return true;
    if (this->x > other.x)
        return false;
    if (this->y >= other.y)
        return false;
    return true;
}

double dot_prod(const Point &p1, const Point &q1, const Point &p2,
                const Point &q2) {
    return (q1.x - p1.x) * (q2.x - p2.x) + (q1.y - p1.y) * (q2.y - p2.y);
}

double dot_prod(const Point &p1, const Point &q1) {
    return p1.x * q1.x + p1.y * q1.y;
}

double cross_prod(const Point &p1, const Point &q1) {
    return p1.x * q1.y - p1.y * q1.x;
}

double cross_prod(const Point &p1, const Point &q1, const Point &p2,
                  const Point &q2) {
    return (q1.x - p1.x) * (q2.y - p2.y) - (q1.y - p1.y) * (q2.x - p2.x);
}

double dist(const Point &p, const Point &q) {
    return std::sqrt((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y));
}

double dist(const Point &p, const Line &l) {
    return std::abs(l.a * p.x + l.b * p.y + l.c) /
           std::sqrt(l.a * l.a + l.b * l.b);
}

double angle(const Point &p, const Point &ref) {
    if (p == ref)
        return -1e-9;
    return std::atan2(p.y - ref.y, p.x - ref.x);
}

double angle(const Point &p1, const Point &q1, const Point &p2,
             const Point &q2) {
    return std::acos(dot_prod(p1, q1, p2, q2) / (dist(p1, q1) * dist(p2, q2)));
}

bool compare_angles(const Point &p, const Point &q, const Point &ref) {
    return angle(p, ref) < angle(q, ref);
}

bool are_colinear(const Point &p, const Point &q, const Point &r) {
    double tolerance = 1e-9; // Adjust tolerance as needed
    return (std::abs(angle(p, q) - angle(p, r)) < tolerance) ||
           std::abs(angle(p, q) + angle(p, r) - M_PI) < tolerance;
}

bool is_convex(const Point &p, const Point &q, const Point &r) {
    return (q.x - p.x) * (r.y - p.y) - (r.x - p.x) * (q.y - p.y) > 0;
}

Line generate_line(const Point &p, const Point &q) {

    if (p.x == q.x) {
        return Line(1, 0, -p.x);
    }

    return Line((q.y - p.y) / (p.x - q.x), 1,
                (q.y * p.x - p.y * q.x) / (q.x - p.x));
}

double Point::norm() { return std::sqrt(x * x + y * y); }

Point Point::operator/(double other) const {
    return Point(this->x / other, this->y / other);
}

Point Point::operator*(double other) const {
    return Point(this->x * other, this->y * other);
}

