#include "points.h"
#include "RandomHull.h"
#include "RandomHullTesting.h"
#include <cassert>
namespace random_hull_testing {  
     
void testing_convex_hull(){

}

void testing_is_visible(){
    Point a(0,0), b(4,0), c(2,3);
    Point d(2.02,1.24), e(1.761, -0.096), f(2.0163, 3.5581);

    random_hull::Edge ab(a,b), bc(b,c), ca(c,a);
    random_hull::Edge ba(b,a), cb(c,b), ac(a,c);

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

void testing_build_c(){

}

void testing_process_ridge(){

}

void testing_join(){
    Point a(4,1), b(6,3), c(3,3);
    random_hull::Edge ab(a,b), ba(b,a);

    printf("\nTesting join...\n");
    assert(random_hull::is_visible(c,ab));

    random_hull::Edge new_edge = random_hull::join(c, a, ab);

    assert(random_hull::is_visible(a, new_edge) == false);

    printf("Test one succeded\n");


}



void testing_merge_sets() {

}

void testing_process_other_ridge() {

}


}
