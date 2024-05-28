#ifndef GRAHAMSCAN_H
#define GRAHAMSCAN_H

#include <vector>
#include <stack>
#include "points.h"


void remove_colinear(std::vector<Point> &points, Point ref);

Point before_top(std::stack<Point> &stack);

std::vector<Point> GrahamScan(std::vector<Point> &points);


#endif
