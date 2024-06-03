#include "ParallelSorting.h"
#include "points.h"
#include <vector>

namespace GrahamScanParallel {

struct UndirectedLinkedPoint;

void Convexify(UndirectedLinkedPoint *begin, UndirectedLinkedPoint *end);

void ConvexHullRec(UndirectedLinkedPoint *begin, UndirectedLinkedPoint *end, int num_points, const int chunk_sz);

std::vector<Point> ConvexHull(std::vector<Point> &points, int NPROC); 

}

