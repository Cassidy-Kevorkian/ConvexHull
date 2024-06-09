#include "RandomHullTesting.h"
#include "RandomHull.h"
#include "algorithm"
#include "points.h"
#include <cassert>
#include <iostream>
#include <random>

static std::default_random_engine generator(100);

namespace random_hull_testing {

void testing_convex_hull() { std::cout << "TESTING: convex_hull" << std::endl; }

void testing_is_visible() {
    Point a(0, 0), b(4, 0), c(2, 3);
    Point d(2.02, 1.24), e(1.761, -0.096), f(2.0163, 3.5581);

    random_hull::Edge ab(a, b), bc(b, c), ca(c, a);
    random_hull::Edge ba(b, a), cb(c, b), ac(a, c);

    printf("\nTesting is_visible...\n");

    assert(random_hull::is_visible(d, ba) == false);
    printf("Test one passed\n");

    assert(random_hull::is_visible(d, ab) == true);
    printf("Test two passed\n");

    assert(random_hull::is_visible(d, cb) == false);
    printf("Test three passed\n");

    assert(random_hull::is_visible(d, bc) == true);
    printf("Test four passed\n");

    assert(random_hull::is_visible(f, ac) == true);
    printf("Test five passed\n");

    assert(random_hull::is_visible(f, ca) == false);
    printf("Test six passed\n");
}

void testing_build_c() {
     //std::cout << "TESTING: build_c" << std::endl;
 //
     //std::map<random_hull::Edge, std::vector<int>> C;
 //
     //std::uniform_real_distribution unif(0., 20.);
 //
     //std::normal_distribution normal_1(0., 0.5);
     //std::normal_distribution normal_2(0., 5.);
 //
     //std::vector<random_hull::Edge> edges = {{Point(0, 0), Point(1, 0)},
                                             //{Point(1, 0), Point(0, 0)},
                                             //{Point(0, 0), Point(1, 1)}};
 //
     //std::vector<std::vector<Point>> points_lists;
 //
     //std::vector<std::vector<int>> visible_correction;
 //
     //for (int i = 0; i < 1000; ++i) {
         //points[0].push_back(Point(unif(generator), normal_1(generator)));
         //points[1].push_back(Point(unif(generator), normal_2(generator)));
         //points[2].push_back(Point(unif(generator), unif(generator)));
 //
         //if (points[O][-1].y > 0) {
             //visible_correction[0].push_back(i);
         //}
 //
         //if (points[1][-1].y < 0) {
             //visible_correction[1].push_back(i);
         //}
 //
         //if (points[2][-1].y > points[2][-1].x) {
             //visible_correction[2].push_back(i);
         //}
     //}
 //
     //for (int i = 0; i < 3; ++i) {
         //build_c(points[i], edges[i], C);
         //sort(C[edges[i]].begin(), C[edges[i]].end());
 //
         //assert(C[edges[i]].size() == visible_correction[i].size());
 //
         //for (int j = 0; j < C[edges[i]].size(); ++j) {
             //assert(C[edges[i]][j] == visible_correction[i][j]);
         //}
 //
         //C[edges[i]] = {};
 //
         //std::cout << "Test " << i << " SUCCESSFUL" << std::endl;
     //}
}

void testing_process_ridge() {
    std::cout << "TESTING: process_ridge" << std::endl;
}

void testing_join() {
    Point a(4, 1), b(6, 3), c(3, 3);
    random_hull::Edge ab(a, b), ba(b, a);

    printf("\nTESTING: join...\n");
    assert(random_hull::is_visible(c, ab));

    random_hull::Edge new_edge = random_hull::join(c, a, ab);

    assert(random_hull::is_visible(a, new_edge) == false);

    printf("Test one succeded\n");
}

void testing_merge_sets() { 

    printf("\nTESTING: merge_sets...\n");
    
    Point A(0,0), B(1,1), C(2,0);
     
    

}

void testing_get_min() { std::cout << "TESTING: get_min" << std::endl; }

void testing_process_other_ridge() {
    std::cout << "TESTING: other_ridge" << std::endl;
}

} // namespace random_hull_testing
