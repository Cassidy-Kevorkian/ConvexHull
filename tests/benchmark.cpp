
#include "QuickHull.h"
#include "QuickHullParallel.h"
#include "points.h"
#include <GrahamScan.h>
#include <fstream>
#include <vector>
void run_file(const std::string &file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening first file" << std::endl;
        return;
    }
    size_t num_points;
    file >> num_points;
    std::vector<Point> points;
    for (size_t i = 0; i < num_points; ++i) {
        double x, y;
        file >> x >> y;
        points.push_back(Point(x, y));
    }
    file.close();

    std::ofstream file1(file_name.substr(0, file_name.length() - 4) +
                        "_GS.txt");
    if (!file1.is_open()) {
        std::cerr << "Error opening file 1" << std::endl;
        return;
    }
    std::vector<Point> convex_hull_graham_scan = GrahamScan(points);
    // std::cout << std::endl << "GrahamScan convex hull:" << std::endl;
    for (Point point : convex_hull_graham_scan) {
        file1 << point << std::endl;
    }
    file1.close();

    std::ofstream file2(file_name.substr(0, file_name.length() - 4) +
                        "_QH.txt");
    if (!file2.is_open()) {
        std::cerr << "Error opening file 2" << std::endl;
        return;
    }
    std::vector<Point> convex_hull_quick_hull = QuickHull(points);
    // std::cout << std::endl << "QuickHull convex hull:" << std::endl;

    for (Point point : convex_hull_quick_hull) {
        file2 << point << std::endl;
    }

    file2.close();

    std::ofstream file3(file_name.substr(0, file_name.length() - 4) +
                        "_QHP.txt");
    if (!file3.is_open()) {
        std::cerr << "Error opening file 3" << std::endl;
        return;
    }

    std::vector<Point> convex_hull_quick_hull_parallel =
        QuickHullParallel(points);
    // std::cout << std::endl << "QuickHullParallel convex hull:" << std::endl;

    for (Point point : convex_hull_quick_hull_parallel) {
        file3 << point << std::endl;
    }

    file3.close();
}

void run_tests() {
    run_file("../tests/test_few.txt");
    run_file("../tests/test_average.txt");
    run_file("../tests/test_many.txt");
}
