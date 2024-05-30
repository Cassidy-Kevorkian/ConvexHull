// generate n concentric points on a circle and add them in tests.txt
#include "tests.h"
#include <GrahamScan.h>
#include <QuickHull.h>
#include <QuickHullParallel.h>
#include <chrono>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <points.h>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>

// main takes two parameters n and the type of test

void test::run_file(const std::string &file_name) {
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
    file1 << convex_hull_graham_scan.size() << std::endl;

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
    file2 << convex_hull_quick_hull.size() << std::endl;

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
    file3 << convex_hull_quick_hull_parallel.size() << std::endl;

    for (Point point : convex_hull_quick_hull_parallel) {
        file3 << point << std::endl;
    }

    file3.close();
}

void test::run_tests() {
      run_file("../tests/test_few.txt");
      run_file("../tests/test_average.txt");
      run_file("../tests/test_many.txt");

    test::check_test("../tests/test_many.txt", "../tests/correction_many.txt", GrahamScan);
}

void test::generate_tests_helper(
    int n, const std::string &type) { // open a file to write the test cases
    double ratio;
    if (type == "few") {
        ratio = 0.1;
    } else if (type == "average") {
        ratio = 0.5;
    } else if (type == "many") {
        ratio = 1;
    } else {
        std::cerr << "Invalid type" << std::endl;
        return;
    }

    std::ofstream file1("../tests/test_" + type + ".txt");
    std::ofstream file2("../tests/correction_" + type + ".txt");
    if (!file1.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }
    if (!file2.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    file1 << n << std::endl;
    std::vector<Point> points;

    int hull_points = n * ratio;
    file2 << hull_points << std::endl;
    double radius = 100.0;
    double x_center = 0.0;
    double y_center = 0.0;

    for (size_t i = 0; i < hull_points; ++i) {
        double x = x_center + radius * std::cos(2 * M_PI * i / hull_points);
        double y = y_center + radius * std::sin(2 * M_PI * i / hull_points);
        // print points to file
        file1 << x << " " << y << std::endl;
        file2 << x << " " << y << std::endl;
    }

    radius = 80;

    for (size_t i = 0; i < n - hull_points; ++i) {
        double angle = ((double)rand() / RAND_MAX) * 2 * M_PI;
        double rand_radius = radius * sqrt((double)rand() / RAND_MAX);

        double x = rand_radius * cos(angle);
        double y = rand_radius * sin(angle);
        // print points to file
        file1 << x << " " << y << std::endl;
    }
    file1.close();
    file2.close();
}

void test::generate_tests(int n) {
    srand(time(0));

    test::generate_tests_helper(n, "few");
    test::generate_tests_helper(n, "average");
    test::generate_tests_helper(n, "many");
}

void test::check_test(const std::string &input_test, const std::string &correction,
                std::vector<Point> F(std::vector<Point> &)) {

    std::ifstream input_file(input_test), correct_file(correction);

    if (!input_file.is_open()) {
        std::cerr << "Error opening first file" << std::endl;
        return;
    }

    int number_tests;

    double a, b;

    input_file >> number_tests;

    std::vector<Point> test_points(number_tests);

    for (size_t i = 0; i < number_tests; ++i) {
        input_file >> a >> b;
        test_points[i] = Point(a, b);
    }

    input_file.close();

    auto start = std::chrono::steady_clock::now();
    std::vector<Point> result = F(test_points);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> timer = end - start;
    std::cout << timer.count() << " ";

    std::unordered_set<Point, pointHash> my_result;
    for (const auto &point : result) {
        my_result.emplace(point);
    }

    std::unordered_set<Point, pointHash> correct_result;
    if (!correct_file.is_open()) {
        std::cerr << "Error opening the second file" << std::endl;
        return;
    }

    correct_file >> number_tests;

    for (size_t i = 0; i < number_tests; ++i) {
        correct_file >> a >> b;
        correct_result.emplace(Point(a, b));
    }

    if (correct_result != my_result) {
        std::cout << "Test failed\n";
    } else {
        std::cout << "Test succeded\n";
    }

    std::cout << correct_result.size() << " " << my_result.size();
    correct_file.close();
}


void test::compare_files(const std::string &file1, const std::string &file2){
    std::ifstream file_a(file1), file_b(file2);

    if(!file_a.is_open()) {
        std::cerr << "Error opening file number 1\n";
        return;
    }


    int line_number; 

    file_a >> line_number;

    if(line_number == 0) {
        throw "Error line_number is 0. Stopping...";
    }

    std::vector<std::pair<double,double>> lines_a(line_number);

    for(size_t i = 0; i < line_number; ++i) {
        file_a >> lines_a[i].first >> lines_a[i].second;
    }

    if(!file_b.is_open()) {
        std::cerr << "Error opening file number 2\n";
        return;
    }


    file_b >> line_number;

    if(line_number == 0) {
        throw "Error line_number is 0. Stopping...";
    }

    std::vector<std::pair<double,double>> lines_b(line_number);

    for(size_t i = 0; i < line_number; ++i) {
        file_b >> lines_b[i].first >> lines_b[i].second;
    }


    std::sort(lines_a.begin(), lines_a.end());
    std::sort(lines_b.begin(), lines_b.end());
    
    printf("Debugging function compare_tests in tests.cpp\n");

    if(lines_a.size() != lines_b.size()) {
        printf("Files do not match\n");
    }

    for(size_t i = 0; i < lines_a.size(); ++i) {
        printf("(%10f,  %10f) || (%10f, %10f)\n", lines_a[i].first, lines_a[i].second, lines_b[i].first, lines_b[i].second);
    }
    std::cout <<"\n";

    
    if(lines_a != lines_b) {
        printf("Files do not match \n");
    } else {
        printf("Files do match \n");
    }

    file_a.close();
    file_b.close();

}

