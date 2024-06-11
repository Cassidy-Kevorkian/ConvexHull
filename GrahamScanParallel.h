#include "ParallelSorting.h"
#include "points.h"
#include <vector>

namespace graham_scan {

struct undirected_linked_point;

void convexify(undirected_linked_point *begin, undirected_linked_point *end);

void find_min(std::vector<Point> &points, int nproc, Point &min_point);

void find_min_thread(std::vector<Point> &points, const int start, const int end, Point &min_point);

void convex_hull_parallel_rec(undirected_linked_point *begin, undirected_linked_point *end, int num_points, const int chunk_sz);

std::vector<Point> convex_hull_parallel(std::vector<Point> &points); 

}

