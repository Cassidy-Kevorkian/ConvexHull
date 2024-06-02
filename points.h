#ifndef POINTS_H
#define POINTS_H
#include <cmath>
#include <iostream>
#include <cstdint>

class Point {
private:
public:
    Point() : x(0), y(0) {}

    Point(double v1, double v2) : x(v1), y(v2) {}

    Point operator-(const Point &other);
    Point operator+(const Point &other);
    bool operator==(const Point &other) const;
    double x, y;

};

class Line {
public:
    Line() : a(0), b(0), c(0) {}

    Line(double v1, double v2, double v3) : a(v1), b(v2), c(v3) {}

    double a, b, c; // Meaning the line ax + by + c = 0
};

// Overload the << operator for Point
std::ostream &operator<<(std::ostream &os, const Point &point);

// Overload the << operator for Line
std::ostream &operator<<(std::ostream &os, const Line &line);

double dot_prod(const Point &p1, const Point &q1);
double dot_prod(const Point &p1, const Point &q1, const Point &p2,
                const Point &q2);

double cross_prod(const Point &p1, const Point &q1, const Point &p2,
                  const Point &q2);
double cross_prod(const Point &p1, const Point &q1);

double dist(const Point &p, const Point &q);

double dist(const Point &p, const Line &l);

double angle(const Point &p, const Point &ref);

double angle(const Point &p1, const Point &q1, const Point &p2,
             const Point &q2);

bool compare_angles(const Point &p, const Point &q, const Point &ref);

bool are_colinear(const Point &p, const Point &q, const Point &r);

bool is_convex(const Point &p, const Point &q, const Point &r);

Line generate_line(const Point &p, const Point &q);

namespace std {

    template<>
    class hash<Point> {
        public:
        std::uint64_t operator()(const Point &p) const {
            return std::hash<double>()(p.x) ^ std::hash<double>()(p.y);
    }
    };

}

#endif
