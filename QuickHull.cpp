#include "QuickHull.h"
#include "points.h"
#include <iostream>
#include <vector>

const double tolerance = 1e-10;

Point quick_hull::furthest_point_from_line(Line &l, std::vector<Point> &points){

    Point furthest_point = points[0];

    for (int i = 1; i < points.size(); ++i) {

        if (dist(furthest_point, l) < dist(points[i], l))
            furthest_point = points[i];
    }

    return furthest_point;
}


std::vector<std::vector<Point>> quick_hull::generate_partition(Point &p, Point &q,
                                                  std::vector<Point> &points){
    std::vector<std::vector<Point>> partition;
    std::vector<Point> part_1, part_2;

    for (int i = 0; i < points.size(); ++i) {

        if (p == points[i] || q == points[i])
            continue;

        if (cross_prod(q - p, points[i] - p) >= 0)
            part_1.push_back(points[i]);

        else
            part_2.push_back(points[i]);
    }

    return {part_1, part_2};
}


std::vector<Point> quick_hull::generate_points_outside(Point &p, Point &q, Point &furthest_point,
                                         std::vector<Point> &points){
    std::vector<Point> points_outside;
    
    for (int i = 0; i < points.size(); ++i) {

        if(furthest_point == points[i]) continue;

        if (cross_prod(q - p, furthest_point - p) * cross_prod(furthest_point - p, points[i] - p) >= -tolerance)
            points_outside.push_back(points[i]);
    }

    return points_outside;
}


void quick_hull::convex_hull_rec(Point &p, Point &q, std::vector<Point> &points,
                  std::vector<Point> &convex_hull) {
    size_t num_points = points.size();

    if (num_points == 0)
        return;

    Line l = generate_line(p, q);
    Point furthest_point = quick_hull::furthest_point_from_line(l, points );
    convex_hull.push_back(furthest_point);

    std::vector<Point> part_1 =
        quick_hull::generate_points_outside(p, q, furthest_point, points );
    std::vector<Point> part_2 =
        quick_hull::generate_points_outside(q, p, furthest_point, points );

	quick_hull::convex_hull_rec(p, furthest_point, part_1, convex_hull);
	quick_hull::convex_hull_rec(furthest_point, q, part_2, convex_hull);
}


std::vector<Point> quick_hull::convex_hull(std::vector<Point> &points) {
    size_t num_points = points.size();
    Point highest_point = points[0], lowest_point = points[0];

    for (int i = 0; i < num_points; ++i) {

        if (highest_point.y < points[i].y)
            highest_point = points[i];

        if (lowest_point.y > points[i].y)
            lowest_point = points[i];
    }

    std::vector<std::vector<Point>> initial_partition =
        quick_hull::generate_partition(lowest_point, highest_point, points);

    std::vector<Point> part_1 = initial_partition[0],
                       part_2 = initial_partition[1];

    std::vector<Point> convex_hull;
    convex_hull.push_back(lowest_point);
    convex_hull.push_back(highest_point);

	quick_hull::convex_hull_rec(lowest_point, highest_point, part_1, convex_hull);
	quick_hull::convex_hull_rec(lowest_point, highest_point, part_2, convex_hull);

    return convex_hull;
}
