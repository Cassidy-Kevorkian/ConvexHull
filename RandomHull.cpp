#include <RandomHull.h>
#include <utility>

std::map<random_hull::Edge, std::vector<Point>> C;
std::mutex C_mtx;

std::set<random_hull::Edge> H;
std::mutex H_mtx;

std::map<Point, std::vector<random_hull::Edge>> M;
std::mutex M_mtx;

std::vector<random_hull::Edge>
random_hull::convex_hull(const std::vector<Point> &points) {
    return {};
};


