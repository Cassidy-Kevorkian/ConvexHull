#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <stack>

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

void remove_colinear(std::vector<Point> &points, Point ref)
{
    std::vector<Point> new_points = {points[0]};

    for (size_t i = 1; i < points.size(); ++i)
    {
        if (are_colinear(ref, new_points.back(), points[i]))
        {
            if (dist(new_points.back(), ref) < dist(points[i], ref))
            {
                new_points[new_points.size() - 1] = points[i];
            }
        }
        else
        {
            new_points.push_back(points[i]);
        }
    }

    points = new_points;
}

Point before_top(std::stack<Point> &stack)
{
    Point top = stack.top();
    stack.pop();
    Point before_top = stack.top();
    stack.push(top);
    return before_top;
}

std::vector<Point> GrahamScan(std::vector<Point> points)
{
    // We first find the point with the least y coordinate, name it P
    Point P = points[0];
    for (auto point : points)
    {
        if (point.y < P.y || (point.y == P.y && point.x < P.x))
            P = point;
    }

    // Sort the points by the angle they make w.r.t. P
    std::sort(points.begin(), points.end(), [&P](const Point &p1, const Point &p2)
              { return compare_angles(p1, p2, P); });

  //  std::cout << P << std::endl;

    remove_colinear(points, P);

    std::stack<Point> stack;
    stack.push(P);
    stack.push(points[0]);

    for (size_t i = 1; i < points.size(); ++i)
    {
        Point point = points[i];
        if (!is_convex(before_top(stack), stack.top(), point))
        {
            stack.pop();
        }
        while (stack.size() > 1 && !is_convex(before_top(stack), stack.top(), point))
        {
            stack.pop();
        }
        stack.push(point);
    }

    std::vector<Point> convexHull;
    while (!stack.empty())
    {
        convexHull.push_back(stack.top());
        stack.pop();
    }

    std::reverse(convexHull.begin(), convexHull.end());

    return convexHull;
}