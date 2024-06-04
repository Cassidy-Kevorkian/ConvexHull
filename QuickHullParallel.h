#ifndef QUICKHULLPARALLEL_H
#define QUICKHULLPARALLEL_H
#include "points.h"
#include <thread>
#include <vector>

void FurthestPointFromLineParallel(const Line &l, std::vector<Point> &points,
                                    size_t start, size_t end, Point &res);

std::vector<std::vector<Point>>
GeneratePartitionParallel(const Point &p, const Point &q, std::vector<Point> &points,
                          size_t num_points);

void GeneratePointsOutsideParallelThread(const Point &p, const Point &q,
                                                 const Point &furthest_point,
                                                 std::vector<Point> &points,
                                                 size_t num_points, std::vector<Point> &points_outside);

void QuickHullParallelRec(const Point &p, const Point &q, std::vector<Point> &points,
                          std::vector<Point> &convex_hull, int rec_depth);

std::vector<Point> QuickHullParallel(std::vector<Point> &points);

#endif
