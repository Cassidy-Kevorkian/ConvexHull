#include "QuickHullParallel.h"
#include "points.h"
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::mutex lock;
int nr_proc = 40;

void quick_hull::furthest_point_from_line_parallel(const Line &l,
                                                   std::vector<Point> &points,
                                                   size_t start, size_t end,
                                                   Point &res) {
    Point furthest_point = points[0];

    for (int i = start; i < end; ++i) {

        if (dist(furthest_point, l) < dist(points[i], l))
            furthest_point = points[i];
    }

    res = furthest_point;
}

std::vector<std::vector<Point>>
quick_hull::generate_partition_parallel(const Point &p, const Point &q,
                                        std::vector<Point> &points,
                                        size_t num_points) {
    std::vector<std::vector<Point>> partition;
    std::vector<Point> part_1, part_2;

    for (int i = 0; i < num_points; ++i) {

        if ((p == points[i]) || (q == points[i]))
            continue;

        if (cross_prod(p, q, p, points[i]) >= 0)
            part_1.push_back(points[i]);

        else
            part_2.push_back(points[i]);
    }

    return {part_1, part_2};
}

void quick_hull::generate_points_outside_parallel(
    const Point &p, const Point &q, const Point &furthest_point,
    std::vector<Point> &points, size_t num_points,
    std::vector<Point> &points_outside) {
    for (int i = 0; i < points.size(); ++i) {

        if (furthest_point == points[i])
            continue;

        if (cross_prod(q - p, furthest_point - p) *
                cross_prod(furthest_point - p, points[i] - p) >=
            0)
            points_outside.push_back(points[i]);
    }
}

void quick_hull::convex_hull_parallel_rec(const Point &p, const Point &q,
                                          std::vector<Point> &points,
                                          std::vector<Point> &convex_hull,
                                          int rec_depth) {
    size_t num_points = points.size();

    if (num_points == 0)
        return;

    Line l = generate_line(p, q);

    int n_threads = nr_proc / (1 << rec_depth);
    Point furthest_point;

    if (n_threads < 2) {
        quick_hull::furthest_point_from_line_parallel(l, points, 0, num_points,
                                                      furthest_point);
    }

    else {

        std::vector<Point> furthest_candidates(n_threads);
        size_t start = 0, end = num_points, increment = num_points / n_threads;
        std::vector<std::thread> threads(n_threads);

        for (size_t i = 0; i < n_threads; ++i) {
            size_t new_end =
                (i == n_threads - 1) ? num_points : start + increment;
            threads[i] =
                std::thread(&quick_hull::furthest_point_from_line_parallel,
                            std::ref(l), std::ref(points), start, new_end,
                            std::ref(furthest_candidates[i]));
            start = new_end;
        }

        for (size_t i = 0; i < n_threads; ++i) {
            threads[i].join();
        }

        quick_hull::furthest_point_from_line_parallel(
            l, furthest_candidates, 0, n_threads, furthest_point);
    }

    lock.lock();
    convex_hull.push_back(furthest_point);
    lock.unlock();

    std::vector<Point> part_1, part_2;
    std::thread th1 =
        std::thread(&quick_hull::generate_points_outside_parallel, std::ref(p),
                    std::ref(q), std::ref(furthest_point), std::ref(points),
                    num_points, std::ref(part_1));
    std::thread th2 =
        std::thread(&quick_hull::generate_points_outside_parallel, std::ref(q),
                    std::ref(p), std::ref(furthest_point), std::ref(points),
                    num_points, std::ref(part_2));
    th1.join();
    th2.join();

    if ((1 << rec_depth) > nr_proc) {
        quick_hull::convex_hull_parallel_rec(p, furthest_point, part_1,
                                             convex_hull, rec_depth);
        quick_hull::convex_hull_parallel_rec(q, furthest_point, part_2,
                                             convex_hull, rec_depth);
        return;
    }

    ++rec_depth;

    std::thread t1 =
        std::thread(&quick_hull::convex_hull_parallel_rec, std::ref(p),
                    std::ref(furthest_point), std::ref(part_1),
                    std::ref(convex_hull), rec_depth);
    std::thread t2 =
        std::thread(&quick_hull::convex_hull_parallel_rec, std::ref(q),
                    std::ref(furthest_point), std::ref(part_2),
                    std::ref(convex_hull), rec_depth);

    t1.join();
    t2.join();
}

std::vector<Point>
quick_hull::convex_hull_parallel(std::vector<Point> &points) {
    size_t num_points = points.size();
    Point highest_point = points[0], lowest_point = points[0];

    for (int i = 0; i < num_points; ++i) {

        if (highest_point.y < points[i].y)
            highest_point = points[i];

        if (lowest_point.y > points[i].y)
            lowest_point = points[i];
    }

    std::vector<std::vector<Point>> initial_partition =
        quick_hull::generate_partition_parallel(lowest_point, highest_point,
                                                points, num_points);
    std::vector<Point> part_1 = initial_partition[0],
                       part_2 = initial_partition[1];
    std::vector<Point> convex_hull;
    convex_hull.push_back(lowest_point);
    convex_hull.push_back(highest_point);
    int rec_depth = 4;
    std::thread t1 =
        std::thread(&quick_hull::convex_hull_parallel_rec,
                    std::ref(lowest_point), std::ref(highest_point),
                    std::ref(part_1), std::ref(convex_hull), rec_depth);
    std::thread t2 =
        std::thread(&quick_hull::convex_hull_parallel_rec,
                    std::ref(lowest_point), std::ref(highest_point),
                    std::ref(part_2), std::ref(convex_hull), rec_depth);

    t1.join();
    t2.join();

    return convex_hull;
}
