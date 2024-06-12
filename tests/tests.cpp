#include "tests.h"
#include <GrahamScan.h>
#include <QuickHull.h>
#include <QuickHullParallel.h>
#include <RandomHull.h>
#include <algorithm>
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

std::vector<int> values_tested = {100, 1000, 10000, 100000, 1000000};
std::default_random_engine
    eng(std::chrono::steady_clock::now().time_since_epoch().count());

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

    std::vector<Point> convex_hull_graham_scan =
        graham_scan::convex_hull(points);
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

    std::vector<Point> convex_hull_quick_hull = quick_hull::convex_hull(points);
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
        quick_hull::convex_hull_parallel(points);
    file3 << convex_hull_quick_hull_parallel.size() << std::endl;

    for (Point point : convex_hull_quick_hull_parallel) {
        file3 << point << std::endl;
    }

    file3.close();
}

void test::robust_tests(std::vector<Point> F(std::vector<Point> &)) {

    for (int i = 0; i < 5; ++i) {

        std::cout << "Testing against circle_test with " << values_tested[i]
                  << " points..." << std::endl;
        test::check_test("../tests/test_files/circle_test_" +
                             std::to_string(values_tested[i]) + ".txt",
                         "../tests/test_files/circle_correction_" +
                             std::to_string(values_tested[i]) + ".txt",
                         F);

         //std::cout << "Testing double_circle_test with " << values_tested[i]
                   //<< " points..." << std::endl;
         //test::check_test("../tests/test_files/double_circle_test_" +
                              //std::to_string(values_tested[i]) + ".txt",
                          //"../tests/test_files/double_circle_correction_" +
                              //std::to_string(values_tested[i]) + ".txt",
                          //F);
 //
         //std::cout << "Testing on square_test with " << values_tested[i]
                   //<< " points..." << std::endl;
         //test::check_test("../tests/test_files/square_test_" +
                              //std::to_string(values_tested[i]) + ".txt",
                          //"../tests/test_files/square_correction_" +
                              //std::to_string(values_tested[i]) + ".txt",
                          //F);
    }
}

void test::generate_circle_test(int num_points) {
    double num_boundary_points;

    if (num_points <= 1000) {
        num_boundary_points = 100;
    }

    else {
        num_boundary_points = 1000;
    }

    std::ofstream test_file("../tests/test_files/circle_test_" +
                            std::to_string(num_points) + ".txt");
    std::ofstream correction_file("../tests/test_files/circle_correction_" +
                                  std::to_string(num_points) + ".txt");

    if (!test_file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    if (!correction_file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    test_file << num_points << std::endl;

    correction_file << num_boundary_points << std::endl;

    double radius = 100.0;
    double x_center = 0.0;
    double y_center = 0.0;

    for (size_t i = 0; i < num_boundary_points; ++i) {
        double x =
            x_center + radius * std::cos(2 * M_PI * i / num_boundary_points);
        double y =
            y_center + radius * std::sin(2 * M_PI * i / num_boundary_points);

        // print points to file
        test_file << x << " " << y << std::endl;
        correction_file << x << " " << y << std::endl;
    }

    for (size_t i = 0; i < num_points - num_boundary_points; ++i) {
        double angle = ((double)rand() / RAND_MAX) * 2 * M_PI;
        double rand_radius = (radius) * ((double)rand() / RAND_MAX) - 1e-1;

        double x = rand_radius * cos(angle);
        double y = rand_radius * sin(angle);
        // print points to file
        test_file << x << " " << y << std::endl;
    }
    test_file.close();
    correction_file.close();
}

void test::generate_double_circle_test(int num_points) {
    double num_boundary_points;

    if (num_points <= 1000) {
        num_boundary_points = 100;
    }

    else {
        num_boundary_points = 1000;
    }

    std::vector<Point> interior_points, exterior_points;

    double radius = 100.0;
    double x_center = 0.0;
    double y_center = 0.0;
    int count = 0;

    for (size_t i = 0; i < num_boundary_points; ++i) {
        double angle = 2 * M_PI * (i / num_boundary_points);

        double x = x_center + radius * std::cos(angle);
        double y = y_center + radius * std::sin(angle);

        if (M_PI / 2 <= angle && 3 * (M_PI / 2) >= angle) {
            exterior_points.push_back(Point(x, y));
            exterior_points.push_back(Point(300 - x, y));
        }

        else {
            interior_points.push_back(Point(x, y));
			interior_points.push_back(Point(300 - x, y));
        }
    }

    for (size_t i = 0; i < num_points - num_boundary_points; ++i) {
        double angle = ((double)rand() / RAND_MAX) * 2 * M_PI;
        double rand_radius = (radius - 20) * ((double)rand() / RAND_MAX);

        double x = rand_radius * cos(angle);
        double y = rand_radius * sin(angle);
        interior_points.push_back(Point(x, y));
        interior_points.push_back(Point(300 - x, y));
    }

    // rotation everything with some random angle

    double rotation_angle = ((double)rand() / RAND_MAX) * 2 * M_PI;
    double c = cos(rotation_angle), s = sin(rotation_angle);

    for (int i = 0; i < interior_points.size(); ++i) {
        Point p = interior_points[i];
        interior_points[i] = Point(p.x * c - p.y * s, p.x * s + p.y * c);
    }

    for (int i = 0; i < exterior_points.size(); ++i) {
        Point p = exterior_points[i];
        exterior_points[i] = Point(p.x * c - p.y * s, p.x * s + p.y * c);
    }

    std::ofstream test_file("../tests/test_files/double_circle_test_" +
                            std::to_string(num_points) + ".txt");
    std::ofstream correction_file(
        "../tests/test_files/double_circle_correction_" +
        std::to_string(num_points) + ".txt");

    if (!test_file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    if (!correction_file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    test_file << exterior_points.size() + interior_points.size() << std::endl;
    correction_file << exterior_points.size() << std::endl;

    for (int i = 0; i < interior_points.size(); ++i) {
        test_file << interior_points[i].x << " " << interior_points[i].y
                  << std::endl;
    }

    for (int i = 0; i < exterior_points.size(); ++i) {
        test_file << exterior_points[i].x << " " << exterior_points[i].y
                  << std::endl;
        correction_file << exterior_points[i].x << " " << exterior_points[i].y
                        << std::endl;
    }

    test_file.close();
    correction_file.close();
}

void test::generate_square_test(int num_points) {

    std::ofstream test_file("../tests/test_files/square_test_" +
                            std::to_string(num_points) + ".txt");
    std::ofstream correction_file("../tests/test_files/square_correction_" +
                                  std::to_string(num_points) + ".txt");

    if (!test_file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    if (!correction_file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    test_file << num_points << std::endl;

    correction_file << 4 << std::endl;

    double side_length = 100.0;

    test_file << 0 << " " << 0 << std::endl;
    correction_file << 0 << " " << 0 << std::endl;
    test_file << 100 << " " << 0 << std::endl;
    correction_file << 100 << " " << 0 << std::endl;
    test_file << 100 << " " << 100 << std::endl;
    correction_file << 100 << " " << 100 << std::endl;
    test_file << 0 << " " << 100 << std::endl;
    correction_file << 0 << " " << 100 << std::endl;

    double radius = 40;

    for (size_t i = 0; i < num_points - 4; ++i) {
        double angle = ((double)rand() / RAND_MAX) * 2 * M_PI;
        double rand_radius = radius * ((double)rand() / RAND_MAX);

        double x = rand_radius * cos(angle);
        double y = rand_radius * sin(angle);
        // print points to file
        test_file << x + 50 << " " << y + 50 << std::endl;
    }

    test_file.close();
    correction_file.close();
}

void test::generate_tests() {
    srand(time(0));

    for (int val : values_tested) {
        std::cout << "Generating circle_test with " << val << " points...";
        test::generate_circle_test(val);
        std::cout << " DONE" << std::endl;

        std::cout << "Generating double_circle_test with " << val
                  << " points...";
        test::generate_double_circle_test(val);
        std::cout << " DONE" << std::endl;

        std::cout << "Generating square_test with " << val << " points...";
        test::generate_square_test(val);
        std::cout << " DONE" << std::endl;
    }
}

void test::check_test(const std::string &input_test,
                      const std::string &correction,
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

    std::shuffle(test_points.begin(), test_points.end(), eng);
    auto start = std::chrono::steady_clock::now();
    std::vector<Point> result = F(test_points);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> timer = end - start;

    std::vector<Point> correct_result;
    if (!correct_file.is_open()) {
        std::cerr << "Error opening the second file" << std::endl;
        return;
    }

    correct_file >> number_tests;

    for (size_t i = 0; i < number_tests; ++i) {
        correct_file >> a >> b;
        correct_result.push_back(Point(a, b));
    }

    std::sort(result.begin(), result.end());
    std::sort(correct_result.begin(), correct_result.end());

    if (result.size() != correct_result.size() || result != correct_result) {
        printf("Files do not match\n");

        for (size_t i = 0; i < result.size(); ++i) {
            if (i >= correct_result.size()) {
                printf("(%10f,  %10f) || (%7sN/A, %7sN/A)\n", result[i].x,
                       result[i].y, "", "");
                continue;
            }
            printf("(%10f,  %10f) || (%10f, %10f)\n", result[i].x, result[i].y,
                   correct_result[i].x, correct_result[i].y);
        }
        printf("%25zu ||  %22zu\n", result.size(), correct_result.size());
        printf("%25s || %22s\n", input_test.c_str(), correction.c_str());

        printf("\n  Files do not match!. Test failed! \n");
    } else {
        printf("\n  Files do match. Test passed! \n");
    }

    printf("\n  EXECUTION TIME : %9f\n\n\n", timer.count());

    // std::cout << "Number of points: " << number_tests << " ";

    correct_file.close();
}

void test::compare_files(const std::string &file1, const std::string &file2) {
    std::ifstream file_a(file1), file_b(file2);

    if (!file_a.is_open()) {
        std::cerr << "Error opening file number 1\n";
        return;
    }

    int line_number;

    file_a >> line_number;

    if (line_number == 0) {
        throw "Error line_number is 0. Stopping...";
    }

    std::vector<std::pair<double, double>> lines_a(line_number);

    for (size_t i = 0; i < line_number; ++i) {
        file_a >> lines_a[i].first >> lines_a[i].second;
    }

    if (!file_b.is_open()) {
        std::cerr << "Error opening file number 2\n";
        return;
    }

    file_b >> line_number;

    if (line_number == 0) {
        throw "Error line_number is 0. Stopping...";
    }

    std::vector<std::pair<double, double>> lines_b(line_number);

    for (size_t i = 0; i < line_number; ++i) {
        file_b >> lines_b[i].first >> lines_b[i].second;
    }

    std::sort(lines_a.begin(), lines_a.end());
    std::sort(lines_b.begin(), lines_b.end());

    printf("Debugging function compare_tests in tests.cpp\n");

    if (lines_a.size() != lines_b.size()) {
        printf("Files do not match\n");
    }

    for (size_t i = 0; i < lines_a.size(); ++i) {
        printf("(%10f,  %10f) || (%10f, %10f)\n", lines_a[i].first,
               lines_a[i].second, lines_b[i].first, lines_b[i].second);
    }
    printf("%25zu ||  %22zu\n", lines_a.size(), lines_b.size());
    printf("%25s || %22s", file1.c_str(), file2.c_str());
    std::cout << "\n";

    if (lines_a != lines_b) {
        printf("Files do not match \n");

    } else {
        printf("Files do match \n");
    }

    file_a.close();
    file_b.close();
}

