// generate n concentric points on a circle and add them in tests.txt
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <points.h>
#include <random>
#include <unordered_set>
#include "tests.h"

// main takes two parameters n and the type of test
namespace test {
void generate_tests_helper(
    int n, const std::string &type) { // open a file to write the test cases
    double ratio;
    if (type == "few") {
        ratio = 0.1;
    } else if (type == "average") {
        ratio = 0.5;
    } else if (type == "many") {
        ratio = 0.9;
    } else {
        std::cerr << "Invalid type" << std::endl;
        return;
    }

    std::ofstream file("../tests/test_" + type + ".txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    file << n << std::endl;
    std::vector<Point> points;

    int hull_points = n * ratio;

    double radius = 10.0;
    double x_center = 0.0;
    double y_center = 0.0;

    for (size_t i = 0; i < hull_points; ++i) {
        double x = x_center + radius * std::cos(2 * M_PI * i / hull_points);
        double y = y_center + radius * std::sin(2 * M_PI * i / hull_points);
        // print points to file
        file << x << " " << y << std::endl;
    }

    radius = 8;

    for (size_t i = 0; i < n - hull_points; ++i) {
        double angle = ((double)rand() / RAND_MAX) * 2 * M_PI;
        double rand_radius = radius * sqrt((double)rand() / RAND_MAX);

        double x = rand_radius * cos(angle);
        double y = rand_radius * sin(angle);
        // print points to file
        file << x << " " << y << std::endl;
    }
    file.close();
}

void generate_tests(int n) {
    srand(time(0));

    generate_tests_helper(n, "few");
    generate_tests_helper(n, "average");
    generate_tests_helper(n, "many");
}

void check_test(const std::string &input_test, const std::string &correction,
                std::vector<Point> F(std::vector<Point> &)) {

    std::ifstream input_file(input_test), correct_file(correction);

    int number_tests;
    double a, b;

    input_file >> number_tests;

    std::vector<Point> test_points(number_tests);

    for (size_t i = 0; i < number_tests; ++i) {
        input_file >> a >> b;
        test_points[i] = Point(a, b);
    }

    std::cout << test_points.size();

    std::vector<Point> result = F(test_points);


    std::unordered_set<Point, pointHash> my_result;
    for (const auto &point : result) {
        my_result.emplace(point);
    }

    std::unordered_set<Point, pointHash> correct_result;

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
    
}


} // namespace test
