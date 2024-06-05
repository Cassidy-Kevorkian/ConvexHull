#include "ParallelSorting.h"
#include "points.h"
#include "GrahamScanParallel.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>


struct GrahamScanParallel::undirected_linked_point {
    undirected_linked_point(Point point) {
        value = point;
        next = NULL;
        prev = NULL;
    }

    Point value;
    GrahamScanParallel::undirected_linked_point *next;
    GrahamScanParallel::undirected_linked_point *prev;
};

void GrahamScanParallel::convexify(GrahamScanParallel::undirected_linked_point *begin, GrahamScanParallel::undirected_linked_point *end) {

    // treating small cases
    if ((begin == end) || (begin->next == end)) {

        return;
    }

    GrahamScanParallel::undirected_linked_point *pred = begin;
    GrahamScanParallel::undirected_linked_point *curr = begin->next;

    while (curr != end) {

        GrahamScanParallel::undirected_linked_point *succ = curr->next;

        while ((pred != begin) && !is_convex(pred->value, curr->value, succ->value)) {
            pred->next = succ;
            succ->prev = pred;
            curr = pred;
            pred = pred->prev;
        }

        pred = pred->next;
        curr = curr->next;
    }
}


void GrahamScanParallel::convex_hull_rec(GrahamScanParallel::undirected_linked_point *begin, GrahamScanParallel::undirected_linked_point *end,
                   int num_points, const int chunk_sz) {

    if (num_points > chunk_sz) {

        int num_points_left = num_points / 2,
            num_points_right = num_points - num_points_left;
        GrahamScanParallel::undirected_linked_point *mid = begin;

        for (int i = 0; i < num_points_left; ++i) {
            mid = mid->next;
        }

        std::thread t1(&convex_hull_rec, begin, mid->prev, num_points_left,
                       chunk_sz);
        std::thread t2(&convex_hull_rec, mid, end, num_points_right, chunk_sz);
        t1.join();
        t2.join();
    }

    convexify(begin, end);
}

std::vector<Point> GrahamScanParallel::convex_hull(std::vector<Point> &points) {
	int NPROC = 14;
    Point P = points[0];
    int num_points = points.size();

    for (int i = 0; i < num_points; ++i) {

        if (points[i].y < P.y || (points[i].y == P.y && points[i].x < P.x)) {
            P = points[i];
        }
    }

    int chunk_sz = num_points / NPROC;
    chunk_sz = std::max(chunk_sz, 1);
	ParallelSorting::sample_sort(points, 0, num_points, chunk_sz, P);
    GrahamScanParallel::undirected_linked_point *root = new GrahamScanParallel::undirected_linked_point(P); // P = points[0]
    GrahamScanParallel::undirected_linked_point *iter = root;

    for (int i = 1; i < num_points; ++i) {
        GrahamScanParallel::undirected_linked_point *helper = new GrahamScanParallel::undirected_linked_point(points[i]);
        iter->next = helper;
        helper->prev = iter;
        iter = iter->next;
    }

    GrahamScanParallel::undirected_linked_point *begin = root, *end = iter;

    // we make sure the linked lists makes a full circle here
    end->next = begin;
    begin->prev = end;

    convex_hull_rec(begin, end, num_points, chunk_sz);

    iter = begin;
    std::vector<Point> convex_hull;

    while (iter != end) {
        convex_hull.push_back(iter->value);
        iter = iter->next;
    }

    convex_hull.push_back(end->value);

    return convex_hull;
}

/*
int main() {
        int n;
        std::cin >> n;
        std::vector<Point> points;
        int NPROC = 8;

        for(int i = 0; i < n; ++i) {
                double a, b;
                std::cin >> a >> b;
                points.push_back(Point(a, b));
        }

        std::vector<Point> convex_hull = GrahamScanParallel::ConvexHull(points,
NPROC);

        for(auto point: convex_hull) {
                std::cout << point << std::endl;
        }

        return 0;
}
*/
