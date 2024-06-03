#include "ParallelSorting.h"
#include "points.h"
#include "GrahamScanParallel.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

namespace GrahamScanParallel {

struct UndirectedLinkedPoint {
    UndirectedLinkedPoint(Point point) {
        value = point;
        next = NULL;
        prev = NULL;
    }

    Point value;
    UndirectedLinkedPoint *next;
    UndirectedLinkedPoint *prev;
};

void Convexify(UndirectedLinkedPoint *begin, UndirectedLinkedPoint *end) {

    // treating small cases
    if ((begin == end) || (begin->next == end)) {

        return;
    }

    UndirectedLinkedPoint *pred = begin;
    UndirectedLinkedPoint *curr = begin->next;

    while (curr != end) {

        UndirectedLinkedPoint *succ = curr->next;

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


void ConvexHullRec(UndirectedLinkedPoint *begin, UndirectedLinkedPoint *end,
                   int num_points, const int chunk_sz) {

    if (num_points > chunk_sz) {

        int num_points_left = num_points / 2,
            num_points_right = num_points - num_points_left;
        UndirectedLinkedPoint *mid = begin;

        for (int i = 0; i < num_points_left; ++i) {
            mid = mid->next;
        }

        std::thread t1(&ConvexHullRec, begin, mid->prev, num_points_left,
                       chunk_sz);
        std::thread t2(&ConvexHullRec, mid, end, num_points_right, chunk_sz);
        t1.join();
        t2.join();
    }

    Convexify(begin, end);
}

std::vector<Point> ConvexHull(std::vector<Point> &points) {
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
    sample_sort(points, 0, num_points, chunk_sz, P);
    UndirectedLinkedPoint *root = new UndirectedLinkedPoint(P); // P = points[0]
    UndirectedLinkedPoint *iter = root;

    for (int i = 1; i < num_points; ++i) {
        UndirectedLinkedPoint *helper = new UndirectedLinkedPoint(points[i]);
        iter->next = helper;
        helper->prev = iter;
        iter = iter->next;
    }

    UndirectedLinkedPoint *begin = root, *end = iter;

    // we make sure the linked lists makes a full circle here
    end->next = begin;
    begin->prev = end;

    ConvexHullRec(begin, end, num_points, chunk_sz);

    iter = begin;
    std::vector<Point> convex_hull;

    while (iter != end) {
        convex_hull.push_back(iter->value);
        iter = iter->next;
    }

    convex_hull.push_back(end->value);

    return convex_hull;
}

} // namespace GrahamScanParallel
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
