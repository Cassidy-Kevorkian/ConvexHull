#ifndef POINTS_H
#define POINTS_H
#include <cmath>
#include <iostream>

struct Point
{
    Point() {
       x = 0.0;
       y = 0.0;
    }

    Point(double v1, double v2) {
        x = v1;
        y = v2;
    }

    double x, y;
};


struct Line
{
    Line() {
        a = 0.0;
        b = 0.0;
        c = 0.0;
    }

    Line(double v1, double v2, double v3) {
        a = v1;
        b = v2;
        c = v3;
    }

    double a, b, c; //Meaning the line ax + by + c = 0
};

// Overload the << operator for Point
std::ostream &operator<<(std::ostream &os, const Point &point);

// Overload the << operator for Line
std::ostream &operator<<(std::ostream &os, const Line &line);

double dot_prod(const Point &p1, const Point &q1, const Point &p2, const Point &q2);

double cross_prod(const Point &p1, const Point &q1, const Point &p2, const Point &q2);

double dist(const Point &p, const Point &q);

double dist(const Point &p, const Line &l);

double angle(const Point &p, const Point &ref);

double angle(const Point &p1, const Point &q1, const Point &p2, const Point &q2);

bool compare_angles(const Point &p, const Point &q, const Point &ref);

bool are_colinear(const Point &p, const Point &q, const Point &r);

bool is_convex(const Point &p, const Point &q, const Point &r);

Line generate_line(const Point &p, const Point &q);

#endif
