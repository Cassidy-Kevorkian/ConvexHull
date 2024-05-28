// generate n concentric points on a circle and add them in tests.txt
#include "../points.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

// main takes two parameters n and the type of test
std::vector<Point> generate_tests(int n, double ratio) {
    std::vector<Point> points;
    double radius = 10.0;
    double x_center = 0.0;
    double y_center = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double x = x_center + radius * std::cos(2 * M_PI * i / n);
        double y = y_center + radius * std::sin(2 * M_PI * i / n);
        points.push_back(Point(x, y));
    }
    // generate n*ratio random points inside the circle
    for (size_t i = 0; i < n * ratio; ++i) {
        double x = (rand() % 2001 - 1000) / 100.0;
        double y = (rand() % 2001 - 1000) / 100.0;
        if (x * x + y * y <= radius * radius) {
            points.push_back(Point(x, y));
        }
    }
    return points;
}
