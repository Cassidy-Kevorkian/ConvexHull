#ifndef QUICKHULLPARALLEL_H
#define QUICKHULLPARALLEL_H
#include "points.h"
#include <thread>
#include <vector>

Point FurthestPointFromLineParallel(const Line &l, std::vector<Point> &points,
                                    size_t num_points);

std::vector<std::vector<Point>>
GeneratePartitionParallel(const Point &p, const Point &q, std::vector<Point> &points,
                          size_t num_points);

std::vector<Point> GeneratePointsOutsideParallel(const Point &p, const Point &q,
                                                 const Point &furthest_point,
                                                 std::vector<Point> &points,
                                                 size_t num_points);

void QuickHullParallelRec(const Point &p, const Point &q, std::vector<Point> &points,
                          std::vector<Point> &convex_hull, int rec_depth);

std::vector<Point> QuickHullParallel(std::vector<Point> &points);

#endif
