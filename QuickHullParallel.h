#ifndef QUICKHULLPARALLEL_H
#define QUICKHULLPARALLEL_H
#include "points.h"
#include "QuickHull.h"
#include <thread>
#include <vector>


namespace quick_hull {
void furthest_point_from_line_parallel(const Line &l, std::vector<Point> &points, size_t start, size_t endl, Point &res);

std::vector<std::vector<Point>> generate_partition_parallel(const Point &p, const Point &q, std::vector<Point> &points, size_t num_points);


void generate_points_outside_parallel(const Point &p, const Point &q,

                                                 const Point &furthest_point,
                                                 std::vector<Point> &points,
                                                 size_t num_points, std::vector<Point> &points_outside);

void convex_hull_parallel_rec(const Point &p, const Point &q, std::vector<Point> &points,
                          std::vector<Point> &convex_hull, int rec_depth);

std::vector<Point> convex_hull_parallel(std::vector<Point> &points);
}

#endif
