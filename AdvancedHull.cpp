#include <points.h>
#include <bits/stdc++.h>
#include <mutex>
#include <thread>

typedef std::pair<Point, Point> edge;

bool is_visible(Point p, edge e) {
    return cross_prod(e.first - p, e.second - p) > 0;
}

std::map<edge, std::vector<Point>> C;
std::mutex C_mtx;

std::set<edge> H;
std::mutex H_mtx;

std::map<Point, std::vector<edge>> M;
std::mutex M_mtx;

void build_C(edge &t, std::vector<Point> &points) {
    for (auto &P : points) {
        if (is_visible(P, t)) {
            C[t].push_back(P);
        }
    }
}

std::vector<Point> AdvancedHull(std::vector<Point> &points) {
    if (points.size() < 3)
        return points;
    std::vector<edge> edges = {edge(points[0], points[1]),
                               edge(points[1], points[2]),
                               edge(points[2], points[0])};
    for (auto &edge : edges) {
        H.insert(edge);
    }
    std::vector<std::thread> workers(3);
    for (int i = 0; i < 3; ++i) {
        workers[i] = std::thread(&build_C, edges[i], std::ref(points));
    }

    for (auto &w : workers) {
        w.join();
    }
}

void ProcessRidge(edge t1, Point r, edge t2) { return; }
