#ifndef QUICKHULL_H
#define QUICKHULL_H

#include "points.h"
#include <vector>

namespace quick_hull {
Point furthest_point_from_line(Line &l, std::vector<Point> &points);

std::vector<std::vector<Point>> generate_partition(Point &p, Point &q,
                                                  std::vector<Point> &points);
                                                  

std::vector<Point> generate_points_outside(Point &p, Point &q, Point &furthest_point,
                                         std::vector<Point> &points);

void convex_hull_rec(Point &p, Point &q, std::vector<Point> &points,
                  std::vector<Point> &convex_hull);

std::vector<Point> convex_hull(std::vector<Point> &points);
}

#endif
