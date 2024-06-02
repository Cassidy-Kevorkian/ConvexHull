#include "points.h"
#include "ParallelSorting.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stack>
#include <vector>
#include <thread>


namespace GrahamParallel {

	struct DoublyLinkedPoint {
		DoublyLinkedPoint(Point point) {
			P = point;
			next = NULL;
			prev = NULL;
		}

		Point P;
		DoublyLinkedPoints *next;
		DoublyLinkedPoints *prev;
	};


	void Convexify(DoublyLinkedPoint *begin, DoublyLinkedPoint *end, int num_points) {
		DoublyLinkedPoint *pred = begin,
						  *curr = begin -> next;

		for (size_t i = 0; i < num_points - 2; ++i) {

			while(pred != begin && !is_convex(pred, curr, curr -> next)) {
				pred -> next = curr -> next;
				(curr -> next) -> prev = pred;
				pred = pred -> prev;
				curr = pred -> next;
			}
				
			curr = curr -> next;
			pred = pred -> next;
		}

	}


	std::vector<*DoublyLinkedPoint> Merge(DoublyLinkedPoint *begin_left, DoublyLinkedPoint *end_left, DoublyLinkedList *begin_right, DoublyLinkedList *end_right) {

		DoublyLinkedPoint *curr = end_left -> prev,
						  *pred = end_left -> prev;
		
		while(pred != end_right) {
			
			while(pred != begin && !is_convex(pred, curr, curr -> next)) {
				pred -> next = curr -> next;
				(curr -> next) -> prev = pred;
				pred = pred -> prev;
				curr = pred -> next;
			}
		
			curr = curr -> next;
			pred = pred -> next;
		}

		return {begin_left, end_right};
		
	}


	std::vector<*DoublyLinkedPoint> ConvexHullRec(DoublyLinkedPoint *begin, DoublyLinkedPoint *end, int num_points, const int chunk_sz) {
		std::vector<*DoublyLinkedPoint> convex_arc;

		if(num_points <= chunk_sz) {
			Convexify(begin, end, num_points);

			return {begin, end};
		}

		int num_points_left = num_points / 2, 
			num_points_right = num_points - num_points - left;
		DoublyLinkedPoint *mid = begin;

		for(int i = 0; i < num_points_left; ++i) {
			mid = mid -> next;
		}

		std::vector<*DoublyLinkedPoint> convex_arc_left = ConvexHullRec(begin, mid -> prev, num_points_left, chunk_sz), 
							            convex_arc_right = ConvexHullRec(mid, end, num_points_right, chunk_sz);

		convex_arc = Merge(convex_arc_left[0], 
						   convex_arc_left[1], 
						   convex_arc_right[0],
						   convex_arc_right[1]);

		return convex_arc;
	}


	std::vector<Point> ConvexHull(std::vector<Point> &points, int NPROC) {
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
		DoublyLinkedPoint *root = new LinkedPoint(P);
		DoublyLinkedPoint *iter = root;

		for(int i = 1; i < num_points; ++i) {
			DoublyLinkedPoint helper = new DoublyLinkedPoint(points[i]);
			iter -> next = helper;
			helper -> prev = iter;
			iter = iter -> next;
		}

		std::vector<*DoublyLinkedPoint> LinkedConvexHull = ConvexHullRec(begin, num_points, chunk_sz);
	}

}


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

