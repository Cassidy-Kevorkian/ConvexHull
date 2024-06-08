#include "points.h"
#include <RandomHull.h>
#include <cstdio>
#include <multimap.h>
#include <multimapTesting.h>
#include <thread>
namespace random_hull_testing {

void testing_entry_set_data() {

    random_hull::Entry<Point, random_hull::Edge> new_entry;

    Point point(10, 2);
    random_hull::Edge edge(Point(1, 2), Point(3, 4));

    new_entry.set_data(point, edge);

    if (!(new_entry.data.first == point)) {
        printf("testing_entry_set_data failed!\n");
        printf("point is not set correctlty\n");
        return;
    }

    if (!(new_entry.data.second == edge)) {
        printf("testing_entry_set_data failed!\n");
        printf("Edge is not set correctly\n");
        return;
    }

    printf("testing_entry_set_data succeded!\n");
}

void testing_entry_get_value() {
    random_hull::Entry<Point, random_hull::Edge> new_entry;

    Point point(10, 2);
    random_hull::Edge edge(Point(1, 2), Point(3, 4));
    new_entry.set_data(point, edge);

    if (!(new_entry.get_value() == edge)) {
        printf("testing_entry_get_value failed!\n");
        printf("Values differ\n");
        return;
    }

    printf("testing_entry_get_value succeded!\n");
}

void testing_entry_get_key() {

    random_hull::Entry<Point, random_hull::Edge> new_entry;

    Point point(10, 2);
    random_hull::Edge edge(Point(1, 2), Point(3, 4));

    new_entry.set_data(point, edge);

    if (!(new_entry.get_key() == point)) {
        printf("testing_entry_get_key failed!\n");
        printf("Keys differ\n");
        return;
    }

    printf("testing_entry_get_key succeded!\n");
}

void testing_multimap_insert_and_set() {
    printf("\nTesting multimap insert and set...\n");

    Point point(10, 2);

    random_hull::Edge edge1(Point(1, 2), Point(3, 4));
    random_hull::Edge edge2(Point(2, 3), Point(0, 4));

    random_hull::multimap<Point, random_hull::Edge> my_map;

    if (my_map.insert_and_set(point, edge1) != true) {
        printf("Test one failed!\n");
    } else {
        printf("Test one succeded!\n");
    }

    if (my_map.insert_and_set(point, edge2)) {
        printf("Test two failed!\n");
    } else {
        printf("Test two succeded!\n");
    }

    // What if we try to insert the same thing twice?
    // We have not implemented this in our code
}

// void testing_multimap_insert_and_set_thread(const Point &point, const
// random_hull::Edge &edge) {
//
// }

void testing_multimap_get_value() {
    printf("\nTesting multimap get_value...\n");

    Point point(10, 2);

    random_hull::Edge edge1(Point(1, 2), Point(3, 4));
    random_hull::Edge edge2(Point(2, 3), Point(0, 4));

    random_hull::multimap<Point, random_hull::Edge> my_map;
    my_map.insert_and_set(point, edge1);
    my_map.insert_and_set(point, edge2);

    if (my_map.get_value(point, edge1) != edge2) {
        printf("Test one failed!\n");
    } else {
        printf("Test one succeded!\n");
    }

    if (my_map.get_value(point, edge2) != edge1) {
        printf("Test two failed!\n");
    } else {
        printf("Test two succeded!\n");
    }

    // What if we try to insert the same thing twice?
    // We have not implemented this in our code
}

void testing_suite() {

    testing_entry_set_data();
    testing_entry_get_value();
    testing_entry_get_key();
    testing_multimap_insert_and_set();
    testing_multimap_get_value();
}

} // namespace random_hull_testing
