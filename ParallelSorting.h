#ifndef PARALLELSORT_H
#define PARALLELSORT_H
#include "points.h"
#include <vector>

namespace parallel_sorting {
void merge(std::vector<Point> &elements, const int left, const int mid,
           const int right, Point &P);

void sample_sort(std::vector<Point> &elements, int left, int right,
                 int chunk_sz, Point &P);
} // namespace parallel_sorting

#endif
