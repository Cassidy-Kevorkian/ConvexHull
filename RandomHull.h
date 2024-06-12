#include <atomic>
#include <limits>
#include <map>
#include <multimap.h>
#include <mutex>
#include <points.h>
#include <set>
#include <vector>
constexpr size_t max_size = std::numeric_limits<size_t>::max();
namespace random_hull {
typedef std::pair<Point, Point> Edge;

struct convex_hull_parameters {

    int cur_threads;
    std::set<random_hull::Edge> &H;
    std::mutex &H_mtx;
    std::map<random_hull::Edge, std::vector<int>> &C;
    std::mutex &C_mtx;
    random_hull::multimap<Point, random_hull::Edge> &M;
    std::vector<Point> &points;
    std::recursive_mutex &thread_lock;

    convex_hull_parameters(int cur_threads, std::set<random_hull::Edge> &H,
                           std::mutex &H_mtx,
                           std::map<random_hull::Edge, std::vector<int>> &C,
                           std::mutex &C_mtx,
                           random_hull::multimap<Point, random_hull::Edge> &M,
                           std::vector<Point> &points,
                           std::recursive_mutex &thread_lock)
        : cur_threads(cur_threads), H(H), H_mtx(H_mtx), C(C), C_mtx(C_mtx),
          M(M), points(points), thread_lock(thread_lock) {}
};

std::vector<Point> convex_hull(std::vector<Point> &points);

std::vector<Point> convex_hull_parallel(std::vector<Point> &points);
std::set<Edge> __convex_hull(convex_hull_parameters *const parameters);

bool is_visible(const Point &p, const Edge &e);

void build_c(Edge &t, const std::vector<Point> &points,
                          std::map<random_hull::Edge, std::vector<int>> &C,
                          std::mutex &C_mtx);

void process_ridge(const Edge &t1, const Point &r, const Edge &t2,
                   convex_hull_parameters *const parameters);

void process_ridge_sequential(const Edge &t1, const Point &r, const Edge &t2,
                              convex_hull_parameters *const parameters);

void merge_sets(const random_hull::Edge &t1,
                             const random_hull::Edge &t2,
                             const random_hull::Edge &t,
                             std::map<random_hull::Edge, std::vector<int>> &C,
                             std::mutex &C_mtx, std::vector<Point> &points);

Edge join(const Point &p, const Point &r, const Edge &t1);

size_t get_min(const std::vector<int> &points_set);

void process_other_ridge(const Point &r1, const Edge &t,
                         convex_hull_parameters *const parameters);

void process_other_ridge_sequential(const Point &r1, const Edge &t,
                                    convex_hull_parameters *const parameters);

std::set<random_hull::Edge>
__convex_hull__sequential(convex_hull_parameters *const parameters);

void print_ridge(const random_hull::Edge &t1, const random_hull::Edge &t2,
                 const Point &r);

void print_edge(const random_hull::Edge &t1);

} // namespace random_hull
// namespace random_hull
std::ostream &operator<<(std::ostream &os, const random_hull::Edge &edge);

