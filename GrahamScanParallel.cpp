
#include "points.h"
#include "ParallelSorting.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stack>
#include <vector>


void remove_colinear(std::vector<Point> &points, Point ref) {
    std::vector<Point> new_points = {points[0]};

    for (size_t i = 1; i < points.size(); ++i) {
        if (are_colinear(ref, new_points.back(), points[i])) {
            if (dist(new_points.back(), ref) < dist(points[i], ref)) {
                new_points[new_points.size() - 1] = points[i];
            }
        } else {
            new_points.push_back(points[i]);
        }
    }

    points = new_points;
}


Point before_top(std::stack<Point> &stack) {
    Point top = stack.top();
    stack.pop();
    Point before_top = stack.top();
    stack.push(top);
    return before_top;
}


std::vector<Point> GrahamScan(std::vector<Point> &points) {
    // We first find the point with the least y coordinate, name it P
    Point P = points[0];
    for (auto point : points) {
        if (point.y < P.y || (point.y == P.y && point.x < P.x))
            P = point;
    }

    // Sort the points by the angle they make w.r.t. P
    std::sort(points.begin(), points.end(),
              [&P](const Point &p1, const Point &p2) {
                  return compare_angles(p1, p2, P);
              });

    //  std::cout << P << std::endl;

    //remove_colinear(points, P);

    std::stack<Point> stack;
    stack.push(P);
    stack.push(points[0]);

    for (size_t i = 1; i < points.size(); ++i) {
        Point point = points[i];
        if (!is_convex(before_top(stack), stack.top(), point)) {
            stack.pop();
        }
        while (stack.size() > 1 &&
               !is_convex(before_top(stack), stack.top(), point)) {
            stack.pop();
        }
        stack.push(point);
    }

    std::vector<Point> convexHull;
    while (!stack.empty()) {
        convexHull.push_back(stack.top());
        stack.pop();
    }

    std::reverse(convexHull.begin(), convexHull.end());

    return convexHull;
}


int main() {
	int n;
	std::cin >> n;
	std::vector<Point> points;
	int NPROC = 2;

	for(int i = 0; i < n; ++i) {
		int a, b;
		std::cin >> a >> b;
		points.push_back(Point(a, b));
	}

	int chunk_sz = n / NPROC;
	chunk_sz = std::max(chunk_sz, 1);
	Point P = points[0];
	sample_sort(points, 0, n, chunk_sz, P);

	for(int i = 0; i < n; ++i) {
		std::cout << points[i] << " ";
	}

	return 0;
}
