#include "ParallelSorting.h"
#include "points.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

void merge(std::vector<Point> &points, const int left, const int mid,
           const int right, Point &P) {
    int i = left, j = mid;
    std::vector<Point> merged_points(right - left);
    int k = 0;

    while (i < mid || j < right) {

        if (i == mid) {
            merged_points[k++] = points[j];
            ++j;
            continue;
        }

        if (j == right) {
            merged_points[k++] = points[i];
            ++i;
            continue;
        }

        if (compare_angles(points[j], points[i], P)) {
            merged_points[k++] = points[j];
            ++j;
            continue;
        }

        merged_points[k++] = points[i];
        ++i;
    }

    for (int i = left; i < right; ++i) {
        points[i] = merged_points[i - left];
    }
}

void sample_sort(std::vector<Point> &points, int left, int right, int chunk_sz,
                 Point &P) {

    if (right - left <= chunk_sz) {
        std::sort(
            points.begin() + left, points.begin() + right,
            [&P](Point &p1, Point &p2) { return compare_angles(p1, p2, P); });

        return;
    }

    int mid = left + (right - left) / 2;
    std::thread t_left(&sample_sort, std::ref(points), left, mid, chunk_sz,
                       std::ref(P));
    std::thread t_right(&sample_sort, std::ref(points), mid, right, chunk_sz,
                        std::ref(P));

    t_left.join(), t_right.join();

    merge(points, left, mid, right, P);
}

/*

int main() {
        int n;
        std::cin >> n;
        std::vector<Point> points;

        for(int i = 0; i < n; ++i) {
                int a, b;
                std::cin >> a >> b;
                points.push_back(Point(a, b));
        }

        int chunk_sz = n / NPROC;
        chunk_sz = std::max(chunk_sz, 1);
        Point P = points[0];
        sample_sort(points, 0, n, chunk_sz, P);

        for(int i = 0; i < n; ++i) {
                std::cout << points[i] << " ";
        }

        return 0;
}


*/
