#include "points.h"
#include "ParallelSorting.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stack>
#include <vector>
#include <thread>


struct LinkedPoint {
	LinkedPoint(Point point) {
		P = point;
		next = NULL;
	}

	Point P;
	LinkedPoints *next;
};

std::vector<Point> GrahamScanParallelRec(std::vector<Point> &points, int NPROC) {

}

std::vector<Point> GrahamScanParallel(std::vector<Point> &points, int NPROC) {
    Point P = points[0];
	int num_points = points.size();

    for (int i = 0; i < num_points; ++i) {

        if (point.y < P.y || (point.y == P.y && point.x < P.x)) {
            P = point;
    	} 

	}
	
	int chunk_sz = num_points / NPROC;
	chunk_sz = std::max(chunk_sz, 1);
	sample_sort(points, 0, n, chunk_sz, P);
	LinkedPoint *root = new LinkedPoint(P);
	LinkedPoint *iter = root;

	for(int i = 0; i < num_points; ++i) {
		iter -> next = new LinkedPoint(points[i]);
		iter = iter -> next;
	}

	LinkedPoint *pred = root;
	LinkedPoint *curr = pred -> next;

    for (size_t i = 1; i < points.size(); ++i) {
        Point point = points[i];
        //if (!is_convex(before_top(stack), stack.top(), point)) {
         //   stack.pop();
        //}
        while (stack.size() > 1 &&
               !is_convex(before_top(stack), stack.top(), point)) {
            stack.pop();
        }
        stack.push(point);
    }

    std::vector<Point> convexHull;
    while (!stack.empty()) {
        convexHull.push_back(stack.top());
        stack.pop();
    }

    std::reverse(convexHull.begin(), convexHull.end());

    return convexHull;
}

/**
int main() {
	int n;
	std::cin >> n;
	std::vector<Point> points;
	int NPROC = 2;

	for(int i = 0; i < n; ++i) {
		double a, b;
		std::cin >> a >> b;
		points.push_back(Point(a, b));
	}

	
	for(int i = 0; i < n; ++i) {
		std::cout << points[i] << " ";
	}

	return 0;
}
*/
