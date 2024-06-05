#include "ParallelSorting.h"
#include "points.h"
#include <vector>

namespace GrahamScanParallel {

struct undirected_linked_point;

void convexify(undirected_linked_point *begin, undirected_linked_point *end);

void convex_hull_rec(undirected_linked_point *begin, undirected_linked_point *end, int num_points, const int chunk_sz);

std::vector<Point> convex_hull(std::vector<Point> &points); 

}

