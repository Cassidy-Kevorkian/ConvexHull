#ifndef QUICKHULL_H
#define QUICKHULL_H

#include "points.h"
#include <vector>

Point FurthestPointFromLine(Line &l, std::vector<Point> &points);

std::vector<std::vector<Point>> GeneratePartition(Point &p, Point &q,
                                                  std::vector<Point> &points);
                                                  

std::vector<Point> GeneratePointsOutside(Point &p, Point &q, Point &furthest_point,
                                         std::vector<Point> &points);

void QuickHullRec(Point &p, Point &q, std::vector<Point> &points,
                  std::vector<Point> &convex_hull);

std::vector<Point> QuickHull(std::vector<Point> &points);

#endif
