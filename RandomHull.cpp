#include "points.h"
#include <RandomHull.h>
#include <algorithm>
#include <cassert>
#include <multimap.h>
#include <mutex>
#include <ostream>
#include <random>
#include <thread>
#include <unordered_set>
#include <utility>
#include <vector>


int NUM_THREADS = 20;

std::set<random_hull::Edge>
random_hull::__convex_hull(convex_hull_parameters *const parameters) {

    const auto &points = parameters->points;
    auto &C = parameters->C;
    auto &C_mtx = parameters->C_mtx;

    if (points.size() <= 3)
        return {};

    std::vector<Edge> edges;

    if (!is_visible(points[2], Edge(points[0], points[1]))) {
        edges.push_back(Edge(points[0], points[1]));
    } else {
        edges.push_back(Edge(points[1], points[0]));
    }

    if (!is_visible(points[0], Edge(points[1], points[2]))) {
        edges.push_back(Edge(points[1], points[2]));
    } else {
        edges.push_back(Edge(points[2], points[1]));
    }

    if (!is_visible(points[1], Edge(points[0], points[2]))) {
        edges.push_back(Edge(points[0], points[2]));
    } else {
        edges.push_back(Edge(points[2], points[0]));
    }

    for (auto &edge : edges) {
        parameters->H.insert(edge);
    }

    std::vector<std::thread> workers(3);

    for (int i = 0; i < 3; ++i) {
        workers[i] =
            std::thread(&random_hull::build_c, std::ref(edges[i]),
                        std::cref(points), std::ref(C), std::ref(C_mtx));
    }

    for (auto &w : workers) {
        w.join();
    }

    parameters->cur_threads = 3;

    for (int i = 0; i < 3; ++i) {
        workers[i] =
            std::thread(random_hull::process_ridge, std::cref(edges[i]),
                        std::cref(points[(i + 1) % 3]),
                        std::cref(edges[(i + 1) % 3]), parameters);
    }

    for (int i = 0; i < 3; ++i) {
        workers[i].join();
    }

    return parameters->H;
}

std::set<random_hull::Edge> random_hull::__convex_hull__sequential(
    convex_hull_parameters *const parameters) {
	NUM_THREADS = 1;
    const auto &points = parameters->points;
    auto &C = parameters->C;
    auto &C_mtx = parameters->C_mtx;
    auto &H = parameters->H;

    if (points.size() <= 3)
        return {};

    std::vector<Edge> edges;

    if (!is_visible(points[2], Edge(points[0], points[1]))) {
        edges.push_back(Edge(points[0], points[1]));
    } else {
        edges.push_back(Edge(points[1], points[0]));
    }

    if (!is_visible(points[0], Edge(points[1], points[2]))) {
        edges.push_back(Edge(points[1], points[2]));
    } else {
        edges.push_back(Edge(points[2], points[1]));
    }

    if (!is_visible(points[1], Edge(points[0], points[2]))) {
        edges.push_back(Edge(points[0], points[2]));
    } else {
        edges.push_back(Edge(points[2], points[0]));
    }

    // printf("\nprinting edges:\n");
    for (auto &edge : edges) {
        H.insert(edge);
    }

    // std::vector<std::thread> workers(3);

    for (int i = 0; i < 3; ++i) {
        build_c(edges[i], points, C, C_mtx);
    }

    // std::cout << C[edges[1]].size() << "\n";
    // std::cout << C[edges[0]].size() << "\n";
    // std::cout << C[edges[2]].size() << "\n";
    //
    for (int i = 0; i < 3; ++i) {
        // std::cout << get_min(C[edges[i]]) << "\n";
        process_ridge_sequential(edges[i], points[(i + 1) % 3],
                                 edges[(i + 1) % 3], parameters);
    }

    return parameters->H;
}

std::vector<Point> random_hull::convex_hull(std::vector<Point> &points) {

    std::atomic<int> cur_threads(0);
    std::map<random_hull::Edge, std::vector<int>> C;
    std::mutex C_mtx;

    std::set<random_hull::Edge> H;
    std::mutex H_mtx;
    std::recursive_mutex thread_lock;

    random_hull::multimap<Point, random_hull::Edge> M(10000000);

    // std::shuffle(points.begin(), points.end(), rng);
    convex_hull_parameters parameters(1, H, H_mtx, C, C_mtx, M, points,
                                      thread_lock);

    random_hull ::__convex_hull__sequential(&parameters);

    std::unordered_set<Point> hash_set;

    for (const auto &x : H) {
        hash_set.insert(x.first);
        hash_set.insert(x.second);
    }

    std::vector<Point> result;

    for (const auto &iter : hash_set) {
        result.push_back(iter);
    }

    return result;
}

std::vector<Point>
random_hull::convex_hull_parallel(std::vector<Point> &points) {
    std::atomic<int> cur_threads;
    std::map<random_hull::Edge, std::vector<int>> C;
    std::mutex C_mtx;

    std::set<random_hull::Edge> H;
    std::mutex H_mtx;
    std::recursive_mutex thread_lock;

    random_hull::multimap<Point, random_hull::Edge> M(1000000);
    convex_hull_parameters parameters(1, H, H_mtx, C, C_mtx, M, points,
                                      thread_lock);

    random_hull ::__convex_hull(&parameters);

    std::unordered_set<Point> hash_set;

    for (const auto &x : H) {
        // std::cout << x.first << " " << x.second << "\n";
        hash_set.insert(x.first);
        hash_set.insert(x.second);
    }

    std::vector<Point> result;

    for (const auto &iter : hash_set) {
        result.push_back(iter);
    }

    return result;
}

void random_hull::build_c(Edge &t, const std::vector<Point> &points,
                          std::map<random_hull::Edge, std::vector<int>> &C,
                          std::mutex &C_mtx) {

    std::vector<int> numbers;

    for (size_t i = 0; i < points.size(); ++i) {

        if (is_visible(points[i], t)) {
            numbers.push_back(i);
        }
    }

    std::lock_guard<std::mutex> lk(C_mtx);
    // std::cout << numbers.size() << "";
    C[t] = std::move(numbers);
}

bool random_hull::is_visible(const Point &p, const Edge &e) {

    return cross_prod(e.first, p, e.second, p) > 0;
    // return cross_prod(e.first - p, e.second - p) > 0;
}

random_hull::Edge random_hull::join(const Point &p, const Point &r,
                                    const Edge &t1) {
    const Point &point1 = t1.first;
    const Point &point2 = t1.second;
    Point other;
    if (point1 == r) {
        other = point2;
    } else {
        other = point1;
    }

    if (!is_visible(other, Edge(p, r))) {
        return Edge(p, r);
    }
    return Edge(r, p);
}

void random_hull::process_ridge_sequential(
    const Edge &t1, const Point &r, const Edge &t2,
    convex_hull_parameters *const parameters) {
    // std::cout << r << "\n";
    // std::cout <<  r <<"\n";
    // std::cout << "A" << "\n";
    // std::cout <<"cur" << "\n";

    auto &C = parameters->C;
    auto &C_mtx = parameters->C_mtx;
    auto &H = parameters->H;
    auto &H_mtx = parameters->H_mtx;
    auto &points = parameters->points;

    if (get_min(C[t1]) == max_size && get_min(C[t2]) == max_size) {
        // std::cout << "1\n";
        return;
    }
    if (get_min(C[t1]) == get_min(C[t2])) {
        std::lock_guard<std::mutex> lk(H_mtx);
        H.erase(t1);
        H.erase(t2);
        return;
    }

    if (get_min(C[t2]) < get_min(C[t1])) {
        process_ridge_sequential(t2, r, t1, parameters);
        return;
    }

    size_t index = get_min(C[t1]);
    // std::cout << H.size() << "\n";
    // for(const auto &t : H) {
    ////std::cout << t << "\n";
    //}
    // std::cout << index << " Muie\n";

    // std::cout << index << " ";
    //  Point p = points[index];
    const Point &p = points[index];

    Edge t = join(p, r, t1);

    merge_sets(t1, t2, t, C, C_mtx, points);

    // std::cout << C[t].size() << "\n";
    // std::cout << get_min(C[t]) << "\n";

    // std::cout << C[t].size() << "\n";

    H_mtx.lock();
    H.erase(t1);
    H.insert(t);
    H_mtx.unlock();

    Point r1 = t.first, r2 = t.second;

    if (r2 == r)
        std::swap(r1, r2);

    // std::thread worker1, worker2;
    std::thread worker1, worker2;
    bool work1 = false, work2 = false;
    parameters->thread_lock.lock();
    if (parameters->cur_threads < NUM_THREADS) {
        worker1 = std::thread(process_ridge, std::cref(t), std::cref(r1),
                              std::cref(t2), parameters);

        work1 = true;
        parameters->thread_lock.unlock();

    } else {
        parameters->thread_lock.unlock();

        process_ridge_sequential(t, r1, t2, parameters);
    }
    parameters->thread_lock.lock();
    if (parameters->cur_threads < NUM_THREADS) {
        worker2 = std::thread(process_other_ridge, std::cref(r2), std::cref(t),
                              parameters);

        work2 = true;
        parameters->thread_lock.unlock();

    } else {
        parameters->thread_lock.unlock();
        process_other_ridge_sequential(r2, t, parameters);
    }

    if (work1) {
        worker1.join();
        parameters->thread_lock.lock();
        --parameters->cur_threads;
        parameters->thread_lock.unlock();
    }


    if (work2) {
        worker2.join();
        parameters->thread_lock.unlock();
        --parameters->cur_threads;
        parameters->thread_lock.unlock();
    }
    // std::cout << "Here\n";
}

void random_hull::process_ridge(const Edge &t1, const Point &r, const Edge &t2,
                                convex_hull_parameters *const parameters) {
    auto &C = parameters->C;
    auto &C_mtx = parameters->C_mtx;
    auto &H = parameters->H;
    auto &H_mtx = parameters->H_mtx;
    auto &points = parameters->points;
    auto &cur_threads = parameters->cur_threads;

    if (get_min(C[t1]) == max_size && get_min(C[t2]) == max_size)
        return;

    if (get_min(C[t1]) == get_min(C[t2])) {
        std::lock_guard<std::mutex> lk(H_mtx);

        H.erase(t1);
        H.erase(t2);

        return;
    }

    if (get_min(C[t2]) < get_min(C[t1])) {
        process_ridge(t2, r, t1, parameters);

        return;
    }

    size_t index = get_min(C[t1]);

    // Point p = points[index];
    const Point &p = points[index];

    Edge t = join(r, p, t1);
    merge_sets(t1, t2, t, C, C_mtx, points);

    std::unique_lock<std::mutex> uq_lk(H_mtx);
    H.erase(t1);
    H.insert(t);
    uq_lk.unlock();

    Point r1 = t.first, r2 = t.second;

    if (r2 == r)
        std::swap(r1, r2);
    std::thread worker1, worker2;
    bool work1 = false, work2 = false;
    // if (cur_threads.load() < NUM_THREADS) {
    int val = NUM_THREADS;
    parameters->thread_lock.lock();
    if (cur_threads < NUM_THREADS) {
        worker1 = std::thread(&process_ridge, std::cref(t), std::cref(r1),
                              std::cref(t2), parameters);
        ++cur_threads;
        parameters->thread_lock.unlock();
        work1 = true;
    } else {
        parameters->thread_lock.unlock();

        process_ridge_sequential(t, r1, t2, parameters);
    }

    // std::cout << cur_threads.load() << "\n" ;
    parameters->thread_lock.lock();
    if (cur_threads < NUM_THREADS) {
        worker2 = std::thread(&process_other_ridge, std::cref(r2), std::cref(t),
                              parameters);
        ++cur_threads;
        parameters->thread_lock.unlock();
        work2 = true;

    } else {
        parameters->thread_lock.unlock();

        process_other_ridge_sequential(r2, t, parameters);
    }

    if (work1) {
        worker1.join();
        parameters->thread_lock.lock();
        --cur_threads;
        parameters->thread_lock.unlock();
    }
    if (work2) {
        worker2.join();
        parameters->thread_lock.lock();
        --cur_threads;
        parameters->thread_lock.unlock();
    }

}

void random_hull::process_other_ridge(
    const Point &r1, const Edge &t, convex_hull_parameters *const parameters) {
    auto &M = parameters->M;
    auto &points = parameters->points;
    if (!M.insert_and_set(r1, t)) {
        const Edge &t1 = M.get_value(r1, t);
        process_ridge(t, r1, t1, parameters);
    }
}

void random_hull::process_other_ridge_sequential(
    const Point &r1, const Edge &t, convex_hull_parameters *const parameters) {
    auto &M = parameters->M;
    auto &points = parameters->points;
    // std::cout << "B" << "\n";
    // std::cout << t.first << " " << t.second << "\n";
    if (!M.insert_and_set(r1, t)) {
        const Edge &t1 = M.get_value(r1, t);

        process_ridge_sequential(t, r1, t1, parameters);
    }
}

size_t random_hull::get_min(const std::vector<int> &points_set) {
    if (points_set.size() == 0) {
        return max_size;
    }

    return points_set[0];
}

void random_hull::merge_sets(const random_hull::Edge &t1,
                             const random_hull::Edge &t2,
                             const random_hull::Edge &t,
                             std::map<random_hull::Edge, std::vector<int>> &C,
                             std::mutex &C_mtx, std::vector<Point> &points) {

    const auto &set1 = C[t1];
    const auto &set2 = C[t2];

    std::vector<int> merged_sets_temp(set1.size() + set2.size());
    std::merge(set1.begin(), set1.end(), set2.begin(), set2.end(),
               merged_sets_temp.begin());

    std::vector<int> merged_sets;

    for (size_t i = 0; i < merged_sets_temp.size(); ++i) {

        const Point &tmp_point = points[merged_sets_temp[i]];

        if (random_hull::is_visible(tmp_point, t)) {
            merged_sets.push_back(merged_sets_temp[i]);
        }
    }

    // std::cout << merged_sets.size() << "\n";

    std::lock_guard<std::mutex> lk(C_mtx);
    C[t] = merged_sets;
}

void random_hull::print_ridge(const random_hull::Edge &t1,
                              const random_hull::Edge &t2, const Point &r) {

    std::cout << "( " << t1.first << " , " << t1.second << " )";
    std::cout << " || ";
    std::cout << r;
    std::cout << " || ";
    std::cout << "( " << t2.first << " , " << t2.second << " )";
    std::cout << "\n\n\n";
}

void random_hull::print_edge(const random_hull::Edge &t1) {}

std::ostream &operator<<(std::ostream &os, const random_hull::Edge &edge) {
    os << " ( " << edge.first << "  -----  " << edge.second << " ) ";
    return os;
}

