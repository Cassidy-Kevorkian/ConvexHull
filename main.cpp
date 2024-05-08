#include <iostream>
#include "GrahamScan.cpp"

int main()
{

    std::vector<Point> points = { {0, 0}, {1, 1}, {2, 2}, {3, 3}, {0, 3}, {-1, 2}, {-2, 1}, {-3, 0} , {2,1}, {-2,3}};
    std::vector ConvexHull = GrahamScan(points);
    for (auto point : ConvexHull){
        std::cout<<point;
    }
    return 0;
}