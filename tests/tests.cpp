// generate n concentric points on a circle and add them in tests.txt
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <points.h>
#include <random>

// main takes two parameters n and the type of test
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
    double radius = 10.0;
    double x_center = 0.0;
    double y_center = 0.0;

    for (size_t i = 0; i < hull_points; ++i) {
        double x = x_center + radius * std::cos(2 * M_PI * i / hull_points);
        double y = y_center + radius * std::sin(2 * M_PI * i / hull_points);
        // print points to file
        file1 << x << " " << y << std::endl;
        file2 << x << " " << y << std::endl;
    }

    radius = 8;

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

void generate_tests(int n) {
    srand(time(0));

    generate_tests_helper(n, "few");
    generate_tests_helper(n, "average");
    generate_tests_helper(n, "many");
}
