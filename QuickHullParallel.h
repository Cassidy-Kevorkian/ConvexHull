#ifndef QUICKHULLPARALLEL_H
#define QUICKHULLPARALLEL_H
#include "points.h"
#include <thread>
#include <vector>

Point FurthestPointFromLineParallel(Line l, std::vector<Point> points,
                                    size_t num_points);

std::vector<std::vector<Point>>
GeneratePartitionParallel(Point p, Point q, std::vector<Point> points,
                          size_t num_points);

std::vector<Point> GeneratePointsOutsideParallel(Point p, Point q,
                                                 Point furthest_point,
                                                 std::vector<Point> points,
                                                 size_t num_points);

void QuickHullParallelRec(Point p, Point q, std::vector<Point> points,
                          std::vector<Point> &convex_hull);

std::vector<Point> QuickHullParallel(std::vector<Point> points);

#endif
