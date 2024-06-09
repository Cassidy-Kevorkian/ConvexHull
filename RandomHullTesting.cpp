#include "RandomHullTesting.h"
#include "RandomHull.h"
#include "algorithm"
#include "points.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

static std::default_random_engine generator(1);

namespace random_hull_testing {

void testing_convex_hull() {
    std::cout << "\nTESTING: convex_hull...\n" << std::endl;
}

void testing_is_visible() {
    Point a(0, 0), b(4, 0), c(2, 3);
    Point d(2.02, 1.24), e(1.761, -0.096), f(2.0163, 3.5581);

    random_hull::Edge ab(a, b), bc(b, c), ca(c, a);
    random_hull::Edge ba(b, a), cb(c, b), ac(a, c);

    printf("\nTESTING: is_visible...\n");

    assert(random_hull::is_visible(d, ba) == false);
    printf("  Test 1 passed\n");

    assert(random_hull::is_visible(d, ab) == true);
    printf("  Test 2 passed\n");

    assert(random_hull::is_visible(d, cb) == false);
    printf("  Test 3 passed\n");

    assert(random_hull::is_visible(d, bc) == true);
    printf("  Test 4 passed\n");

    assert(random_hull::is_visible(f, ac) == true);
    printf("  Test 5 passed\n");

    assert(random_hull::is_visible(f, ca) == false);
    printf("  Test 6 passed\n");
}

void testing_build_c() {
    std::cout << "\nTESTING: build_c...\n" << std::endl;
    std::uniform_real_distribution unif(-20., 20.);
    std::normal_distribution normal_1(0., 0.5);
    std::normal_distribution normal_2(0., 5.);

    std::map<random_hull::Edge, std::vector<int>> D;

    std::vector<random_hull::Edge> edges = {
        {Point(0, 0), Point(1, 0)},
        {Point(1, 0), Point(0, 0)},
        {Point(0, 0), Point(1, 1)}};

    std::vector<std::vector<Point>> points(3);

    std::vector<std::vector<int>> visible_correction(3);

    for (int i = 0; i < 10000; ++i) {
        points[0].push_back(
            Point(unif(generator), normal_1(generator)));
        points[1].push_back(
            Point(unif(generator), normal_2(generator)));
        points[2].push_back(Point(unif(generator), unif(generator)));

        if (points[0][i].y > 0) {
            visible_correction[0].push_back(i);
        }

        if (points[1][i].y < 0) {
            visible_correction[1].push_back(i);
        }

        if (points[2][i].y > points[2][i].x) {
            visible_correction[2].push_back(i);
        }
    }

    for (int i = 0; i < 3; ++i) {
        random_hull::build_c(edges[i], points[i], D);
        assert(D[edges[i]].size() == visible_correction[i].size());

        for (int j = 0; j < D[edges[i]].size(); ++j) {
            assert(D[edges[i]][j] == visible_correction[i][j]);
        }

        D[edges[i]] = {};

        std::cout << "  Test " << i + 1 << " passed" << std::endl;
    }
}

void testing_process_ridge() {
    std::cout << "\nTESTING: process_ridge...\n" << std::endl;
}

void testing_join() {
    Point a(4, 1), b(6, 3), c(3, 3);
    random_hull::Edge ab(a, b), ba(b, a);

    printf("\nTESTING: join...\n");
    assert(random_hull::is_visible(c, ab));

    random_hull::Edge new_edge = random_hull::join(c, a, ab);

    assert(random_hull::is_visible(a, new_edge) == false);

    printf("  Test 1 passed\n");
}

struct PointComparator {
    bool operator()(const random_hull::Edge &lhs,
                    const random_hull::Edge &rhs) const {
        if (lhs.first < rhs.first)
            return true;
        if ((rhs.first < lhs.first))
            return false;
        return lhs.second < rhs.second;
    }
};

void testing_merge_sets() {

    printf("\nTESTING: merge_sets...\n");

    std::uniform_real_distribution unif(0., 1.);
    Point A(0, 0), B(1, 1), C(2, 0);
    random_hull::Edge AB(A, B), BC(B, C);

    std::vector<Point> points;
    std::vector<int> set_AB, set_BC;

    double thetaAB = cross_prod(Point(1, 0), B - A) / (B - A).norm();
    double thetaBC = cross_prod(Point(1, 0), C - B) / (C - B).norm();

    std::map<random_hull::Edge, std::vector<int>> C_test;

    double ab_norm = (A - B).norm(), bc_norm = (B - C).norm();
    for (int i = 0; i < 10000; ++i) {
        if (unif(generator) < 0.5) {
            double u1 = unif(generator), u2 = unif(generator);

            double z0 =
                ab_norm / 2.05 * u1 * std::cos(M_PI * u2 + thetaAB);
            double z1 =
                ab_norm / 2.05 * u1 * std::sin(M_PI * u2 + thetaAB);

            points.push_back(Point(z0, z1) + (A + B) / 2.);
            set_AB.push_back(i);
        } else {
            double u1 = unif(generator), u2 = unif(generator);

            double z0 =
                bc_norm / 2.05 * u1 * std::cos(M_PI * u2 + thetaBC);
            double z1 =
                bc_norm / 2.05 * u1 * std::sin(M_PI * u2 + thetaBC);

            points.push_back(Point(z0, z1) + (B + C) / 2);
            set_BC.push_back(i);
        }
    }

    Point P(0.8, 1.8);
    random_hull::Edge PB(P, B);

    C_test[AB] = set_AB;
    C_test[BC] = set_BC;

    random_hull::merge_sets(C_test, AB, BC, PB, points);


    assert(C_test[PB] == set_BC);
    printf("  Test 1 passed\n");
}


void testing_process_other_ridge() {
    std::cout << "TESTING: other_ridge" << std::endl;
}

} // namespace random_hull_testing
