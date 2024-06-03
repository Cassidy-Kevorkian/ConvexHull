#include <GrahamScan.h>
#include <GrahamScanParallel.h>
#include <QuickHull.h>
#include <QuickHullParallel.h>
#include <points.h>
#include <ParallelSorting.h>
#include <tests/tests.h>

#define TEST 0    // 0 for automated tests, 1 for user input
#define CORRECT 0 // 1 for correctness check

int main() {
    // Tests
    test::generate_tests(5000);
    //test::run_tests();
     //test::compare_files("../tests/test_many_QH.txt",
                         //"../tests/correction_many.txt");

    test::check_test("../tests/test_average.txt", "../tests/correction_average.txt", GrahamScanParallel::ConvexHull);

    return 0;

    // // User input
    // std::cout << "Please enter the number of points:" << std::endl;
    // size_t num_points;
    // std::cin >> num_points;
    // std::cout << "Enter the points one by one in the  format x y:" <<
    // std::endl; std::vector<Point> points;

    // for(int i = 0; i < num_points; ++i) {
    //     double x, y;
    //     std::cin >> x;
    //     std::cin >> y;
    //     points.push_back(Point(x, y));
    // }

    // std::vector<Point> convex_hull_graham_scan = GrahamScan(points);
    // std::cout << std::endl << "GrahamScan convex hull:" << std::endl;

    // for(Point point : convex_hull_graham_scan) {
    //     std::cout << point << std::endl;
    // }

    // std::vector<Point> convex_hull_quick_hull = QuickHull(points);
    // std::cout << std::endl << "QuickHull convex hull:" << std::endl;

    // for(Point point : convex_hull_quick_hull) {
    //     std::cout << point << std::endl;
    // }

    // std::vector<Point> convex_hull_quick_hull_parallel =
    // QuickHullParallel(points); std::cout << std::endl << "QuickHullParallel
    // convex hull:" << std::endl;

    // for(Point point : convex_hull_quick_hull_parallel) {
    //     std::cout << point << std::endl;
    // }
    // }
}

/*

SOME TEST CASES:

1)
INPUT:
8
7 7 7 -7 -7 -7 -7 7 9 0 -9 0 0 9 0 -9

EXPECTED OUTPUT:
(0, -9) (7, -7) (9, 0) (7, 7) (0, 9) (-7, 7) (-9, 0) (-7, -7)


2)
INPUT:
16
7 7 7 -7 -7 -7 -7 7 9 0 -9 0 0 9 0 -9 0 0 1 2 -2 1 -1 -1 3 4 4 3 -5 4 6 5

EXPECTED OUTPUT:
(0, -9) (7, -7) (9, 0) (7, 7) (0, 9) (-7, 7) (-9, 0) (-7, -7)


3)
INPUT:
72
0 0 1 2 -2 1 -1 -1 3 4 4 3 -5 4 6 5 7 7 7 -7 -7 -7 -7 7 9 0 -9 0 0 9 0 -9 -8 0 8
0 -7 0 7 0 -6 0 6 0 -5 0 5 0 -4 0 4 0 -3 0 3 0 -2 0 2 0 -1 0 1 0 0 -8 0 8 0 -7 0
7 0 -6 0 6 0 -5 0 5 0 -4 0 4 0 -3 0 3 0 -2 0 2 0 -1 0 1 1 1 2 2 3 3 4 4 5 5 6 6
1 -1 2 -2 3 -3 4 -4 5 -5 6 -6 -1 1 -2 2 -3 3 -4 4 -5 5 -6 6 -1 -1 -2 -2 -3 -3 -4
-4 -5 -5 -6 -6

EXPECTED OUTPUT:
(0, -9) (7, -7) (9, 0) (7, 7) (0, 9) (-7, 7) (-9, 0) (-7, -7)


4)
INPUT:
40
0.3215348546593775 0.03629583077160248
0.02402358131857918 -0.2356728797179394
0.04590851212470659 -0.4156409924995536
0.3218384001607433 0.1379850698988746
0.11506479756447 -0.1059521474930943
0.2622539999543261 -0.29702873322836
-0.161920957418085 -0.4055339716426413
0.1905378631228002 0.3698601009043493
0.2387090918968516 -0.01629827079949742
0.07495888748668034 -0.1659825110491202
0.3319341836794598 -0.1821814101954749
0.07703635755650362 -0.2499430638271785
0.2069242999022122 -0.2232970760420869
0.04604079532068295 -0.1923573186549892
0.05054295812784038 0.4754929463150845
-0.3900589168910486 0.2797829520700341
0.3120693385713448 -0.0506329867529059
0.01138812723698857 0.4002504701728471
0.009645149586391732 0.1060251100976254
-0.03597933197019559 0.2953639456959105
0.1818290866742182 0.001454397571696298
0.444056063372694 0.2502497166863175
-0.05301752458607545 -0.06553921621808712
0.4823896228171788 -0.4776170002088109
-0.3089226845734964 -0.06356112199235814
-0.271780741188471 0.1810810595574612
0.4293626522918815 0.2980897964891882
-0.004796652127799228 0.382663812844701
0.430695573269106 -0.2995073500084759
0.1799668387323309 -0.2973467472915973
0.4932166845474547 0.4928094162538735
-0.3521487911717489 0.4352656197131292
-0.4907368011686362 0.1865826865533206
-0.1047924716070224 -0.247073392148198
0.4374961861758457 -0.001606279519951237
0.003256207800708899 -0.2729194320486108
0.04310378203457577 0.4452604050238248
0.4916198379282093 -0.345391701297268
0.001675087028811806 0.1531837672490476
-0.4404289572876217 -0.2894855991839297


EXPECTED OUTPUT:
(-0.161920957418085, -0.4055339716426413)
(0.05054295812784038, 0.4754929463150845)
(0.4823896228171788, -0.4776170002088109)
(0.4932166845474547, 0.4928094162538735)
(-0.3521487911717489, 0.4352656197131292)
(-0.4907368011686362, 0.1865826865533206)
(0.4916198379282093, -0.345391701297268)
(-0.4404289572876217, -0.2894855991839297)

*/
