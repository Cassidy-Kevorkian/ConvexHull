#include <RandomHull.h>
#include <multimap.h>
#include <thread>
#include <utility>

std::map<random_hull::Edge, std::vector<Point>> C;
std::mutex C_mtx;

std::set<random_hull::Edge> H;
std::mutex H_mtx;

random_hull::multimap<Point, random_hull::Edge> M;
std::mutex M_mtx;

std::vector<random_hull::Edge>
random_hull::convex_hull(const std::vector<Point> &points) {
    if (points.size() <= 3)
        return {};

    std::vector<Edge> edges = {Edge(points[0], points[1]),
                               Edge(points[1], points[2]),
                               Edge(points[2], points[0])};

    for (auto &edge : edges) {
        H.insert(edge);
    }

    std::vector<std::thread> workers(3);

     for (int i = 0; i < 3; ++i) {
        workers[i] = std::thread(&random_hull::build_C, std::ref(edges[i]), std::cref(points));
     }

    for (auto &w : workers) {
        w.join();
    }
}

void random_hull::build_C(Edge &t, const std::vector<Point> &points) {

    for (const auto &P : points) {
        if (is_visible(P, t)) {
            C[t].push_back(P);
        }
    }
}

bool random_hull::is_visible(const Point &p, const Edge &e) {
    return cross_prod(e.first - p, e.second - p) > 0;
}

void random_hull::ProcessRidge(const Edge &t1, const Point &r, const Edge &t2) {}

