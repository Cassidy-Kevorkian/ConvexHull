#ifndef QUICKHULL_H
#define QUICKHULL_H

#include <vector>
#include "points.h"

Point FurthestPointFromLine(Line l, std::vector<Point> points, size_t num_points);

std::vector<std::vector<Point>> GeneratePartition(Point p, Point q, std::vector<Point> points, size_t num_points);

std::vector<Point> GeneratePointsOutside(Point p, Point q, Point furthest_point, std::vector<Point> points, size_t num_points);

void QuickHullRec(Point p, Point q, std::vector<Point> points, std::vector<Point> &convex_hull);

std::vector<Point> QuickHull(std::vector<Point> points);

#endif
