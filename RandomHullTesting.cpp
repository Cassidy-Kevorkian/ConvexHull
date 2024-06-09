#include "RandomHullTesting.h"
#include "points.h"
#include <iostream>
#include <random>
#include "algorithm"
#include <cassert>

static std::default_random_engine generator(100);

namespace random_hull_testing {  
     
void testing_convex_hull(){
	std::cout << "TESTING: convex_hull" << std::endl;

}

void testing_is_visible(){

}

void testing_build_c(){
	std::cout << "TESTING: build_c" << std::endl;

	std::map<randomHull::Edge, std::vector<int>> C;

	std::uniform_real_distribution unif(0, 20);

	std::normal_distribution normal_1(0, 0.5);
	std::normal_distribution normal_2(0, 5);

	Edge edges = {{Point(0, 0), Point(1, 0)},  {Point(1, 0), Point(0, 0)}, {Point(0, 0), Point(1, 1)}};

	std::vector<std::vector<Point>> points_lists;
	
	std::vector<std::vector<int>> visible_correction;

	for(int i = 0; i < 1000; ++i) {
		points[0].push_back(Point(unif(generator), normal_1(generator)));
		points[1].push_back(Point(unif(generator), normal_2(generator)));
		points[2].push_back(Point(unif(generator), unif(generator)));

		if(points[O][-1].y > 0) {
			visible_correction[0].push_back(i);
		}

		if(points[1][-1].y < 0) {
			visible_correction[1].push_back(i);
		}

		if(points[2][-1].y > points[2][-1].x) {
			visible_correction[2].push_back(i);
		}

	}

	for(int i = 0; i < 3; ++i) {
		build_c(points[i], edges[i], C);
		sort(C[edges[i]].begin(), C[edges[i]].end());

		assert(C[edges[i]].size() == visible_correction[i].size());

		for(int j = 0; j < C[edges[i]].size(); ++j) {
			assert(C[edges[i]][j] == visible_correction[i][j]);
		}

		C[edges[i]] = {};

		std::cout << "Test " << i << " SUCCESSFUL" << std::endl;
	}


}


void testing_process_ridge(){
	std::cout << "TESTING: process_ridge" << std::endl;

}

void testing_join(){
	std::cout << "TESTING: join" << std::endl;

}

void testing_get_min(){
	std::cout << "TESTING: get_min" << std::endl;

}

void testing_process_other_ridge() {
	std::cout << "TESTING: other_ridge" << std::endl;

}

}
