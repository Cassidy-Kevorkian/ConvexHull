#include "QuickHullParallel.h"
#include "points.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex lock;

void FurthestPointFromLineParallel(const Line &l, std::vector<Point> &points,
                                    size_t start, size_t end, Point &res) {
    Point furthest_point = points[0];

    for (int i = start; i < end; ++i) {

        if (dist(furthest_point, l) < dist(points[i], l))
            furthest_point = points[i];
    }

    res = furthest_point;
}

std::vector<std::vector<Point>>
GeneratePartitionParallel(const Point &p, const Point &q, std::vector<Point> &points,
                          size_t num_points) {
    std::vector<std::vector<Point>> partition;
    std::vector<Point> part_1, part_2;

    for (int i = 0; i < num_points; ++i) {

        if (((p.x == points[i].x) && (p.y == points[i].y)) ||
            ((q.x == points[i].x) && (q.y == points[i].y)))
            continue;

        if (cross_prod(p, q, p, points[i]) >= 0)
            part_1.push_back(points[i]);

        else
            part_2.push_back(points[i]);
    }

    return {part_1, part_2};
}


void GeneratePointsOutsideParallelThread(const Point &p, const Point &q,
                                                 const Point &furthest_point,
                                                 std::vector<Point> &points,
                                                 size_t num_points, std::vector<Point> &points_outside) {
    for (int i = 0; i < points.size(); ++i) {

        if(furthest_point == points[i]) continue;

        if (cross_prod(q - p, furthest_point - p) * cross_prod(furthest_point - p, points[i] - p) >= 0)
            points_outside.push_back(points[i]);
    }
}

void QuickHullParallelRec(const Point &p, const Point &q, std::vector<Point> &points,
                          std::vector<Point> &convex_hull, int rec_depth) {
    size_t num_points = points.size();

    if (num_points == 0)
        return;

    Line l = generate_line(p, q);
    size_t start = 0, end = num_points, middle = num_points / 2;
    Point furthest_left, furthest_right, furthest_point;

    std::thread thr1 = std::thread(&FurthestPointFromLineParallel, std::ref(l),
                                 std::ref(points), start, middle, std::ref(furthest_left));
    std::thread thr2 = std::thread(&FurthestPointFromLineParallel, std::ref(l),
                                 std::ref(points), middle, end, std::ref(furthest_right));
    thr1.join();
    thr2.join();

    if (dist(furthest_left, l) > dist(furthest_right, l))
        furthest_point = furthest_left;
    else
        furthest_point = furthest_right;
   // Point furthest_point = FurthestPointFromLineParallel(l, points, num_points);
    lock.lock();
    convex_hull.push_back(furthest_point);
    lock.unlock();

    std::vector<Point> part_1, part_2;
    std::thread th1 = std::thread(&GeneratePointsOutsideParallelThread, std::ref(p),
                                 std::ref(q), std::ref(furthest_point),
                                 std::ref(points), num_points, std::ref(part_1));
    std::thread th2 = std::thread(&GeneratePointsOutsideParallelThread, std::ref(q), std::ref(p),
                                 std::ref(furthest_point), std::ref(points), num_points, std::ref(part_2));
    th1.join();
    th2.join();

    
    if(rec_depth > 4) {
        QuickHullParallelRec(p, furthest_point, part_1, convex_hull, rec_depth);
        QuickHullParallelRec(q, furthest_point, part_2, convex_hull, rec_depth);
        return;
    }
    
    ++rec_depth;

    std::thread t1 = std::thread(&QuickHullParallelRec, std::ref(p),
                                 std::ref(furthest_point), std::ref(part_1),
                                 std::ref(convex_hull), rec_depth);
    std::thread t2 = std::thread(&QuickHullParallelRec, std::ref(q),
                                 std::ref(furthest_point), std::ref(part_2),
                                 std::ref(convex_hull), rec_depth);

    t1.join();
    t2.join();
}

std::vector<Point> QuickHullParallel(std::vector<Point> &points) {
    size_t num_points = points.size();
    Point highest_point = points[0], lowest_point = points[0];

    for (int i = 0; i < num_points; ++i) {

        if (highest_point.y < points[i].y)
            highest_point = points[i];

        if (lowest_point.y > points[i].y)
            lowest_point = points[i];
    }

    std::vector<std::vector<Point>> initial_partition =
        GeneratePartitionParallel(lowest_point, highest_point, points,
                                  num_points);
    std::vector<Point> part_1 = initial_partition[0],
                       part_2 = initial_partition[1];
    std::vector<Point> convex_hull;
    convex_hull.push_back(lowest_point);
    convex_hull.push_back(highest_point);
    int rec_depth = 4;
    std::thread t1 = std::thread(&QuickHullParallelRec, std::ref(lowest_point),
                                 std::ref(highest_point), std::ref(part_1),
                                 std::ref(convex_hull), rec_depth);
    std::thread t2 = std::thread(&QuickHullParallelRec, std::ref(lowest_point),
                                 std::ref(highest_point), std::ref(part_2),
                                 std::ref(convex_hull), rec_depth);

    t1.join();
    t2.join();

    return convex_hull;
}
