#include "GrahamScanParallel.h"
#include "ParallelSorting.h"
#include "points.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

struct graham_scan::undirected_linked_point {
    undirected_linked_point(Point point) {
        value = point;
        next = NULL;
        prev = NULL;
    }

    Point value;
    graham_scan::undirected_linked_point *next;
    graham_scan::undirected_linked_point *prev;
};

void graham_scan::convexify(graham_scan::undirected_linked_point *begin,
                            graham_scan::undirected_linked_point *end) {

    // treating small cases
    if ((begin == end) || (begin->next == end)) {

        return;
    }

    graham_scan::undirected_linked_point *pred = begin;
    graham_scan::undirected_linked_point *curr = begin->next;

    while (curr != end) {

        graham_scan::undirected_linked_point *succ = curr->next;

        while ((pred != begin) &&
               !is_convex(pred->value, curr->value, succ->value)) {
            pred->next = succ;
            succ->prev = pred;
            curr = pred;
            pred = pred->prev;
        }

        pred = pred->next;
        curr = curr->next;
    }
}

void graham_scan::convex_hull_parallel_rec(
    graham_scan::undirected_linked_point *begin,
    graham_scan::undirected_linked_point *end, int num_points,
    const int chunk_sz) {

    if (num_points > chunk_sz) {

        int num_points_left = num_points / 2,
            num_points_right = num_points - num_points_left;
        graham_scan::undirected_linked_point *mid = begin;

        for (int i = 0; i < num_points_left; ++i) {
            mid = mid->next;
        }

        std::thread t1(&convex_hull_parallel_rec, begin, mid->prev,
                       num_points_left, chunk_sz);
        std::thread t2(&convex_hull_parallel_rec, mid, end, num_points_right,
                       chunk_sz);
        t1.join();
        t2.join();
    }

    convexify(begin, end);
}

void graham_scan::find_min(std::vector<Point> &points, int nproc,
                           Point &min_point) {
    int num_points = points.size();
    int chunk_sz = num_points / nproc;

    std::vector<std::thread> threads(nproc);
    std::vector<Point> min_points(nproc);

    int start = 0, end = num_points;

    for (int i = 0; i < nproc; ++i) {
        int new_end = (i == nproc - 1) ? num_points : start + chunk_sz;
        threads[i] = std::thread(graham_scan::find_min_thread, std::ref(points),
                                 start, end, std::ref(min_points[i]));
        start = new_end;
    }
    for (int i = 0; i < nproc; ++i) {
        threads[i].join();
    }

    graham_scan::find_min_thread(min_points, 0, nproc, min_point);
}

void graham_scan::find_min_thread(std::vector<Point> &points, const int start,
                                  const int end, Point &min_point) {
    Point P = points[start];
    for (int i = start; i < end; ++i) {
        if (points[i].y < P.y || (points[i].y == P.y && points[i].x < P.x)) {
            P = points[i];
        }
    }
    min_point = P;
}

std::vector<Point>
graham_scan::convex_hull_parallel(std::vector<Point> &points) {
    int NPROC = 20;
    // Point P = points[0];
    int num_points = points.size();

    Point P;
    graham_scan::find_min(points, NPROC, P);

    int chunk_sz = num_points / NPROC;
    chunk_sz = std::max(chunk_sz, 1);
    parallel_sorting::sample_sort(points, 0, num_points, chunk_sz, P);
    graham_scan::undirected_linked_point *root =
        new graham_scan::undirected_linked_point(P); // P = points[0]
    graham_scan::undirected_linked_point *iter = root;

    for (int i = 1; i < num_points; ++i) {
        graham_scan::undirected_linked_point *helper =
            new graham_scan::undirected_linked_point(points[i]);
        iter->next = helper;
        helper->prev = iter;
        iter = iter->next;
    }

    graham_scan::undirected_linked_point *begin = root, *end = iter;

    // we make sure the linked lists makes a full circle here
    end->next = begin;
    begin->prev = end;

    convex_hull_parallel_rec(begin, end, num_points, chunk_sz);

    iter = begin;
    std::vector<Point> convex_hull;

    while (iter != end) {
        convex_hull.push_back(iter->value);
        iter = iter->next;
    }

    convex_hull.push_back(end->value);

    return convex_hull;
}

