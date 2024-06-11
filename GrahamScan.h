#ifndef GRAHAMSCAN_H
#define GRAHAMSCAN_H

#include <vector>
#include <stack>
#include "points.h"

namespace graham_scan {
void remove_colinear(std::vector<Point> &points, Point ref);

Point before_top(std::stack<Point> &stack);

std::vector<Point> convex_hull(std::vector<Point> &points);
}


#endif
