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
std::set<Edge> __convex_hull(const std::vector<Point> &points);
std::vector<Point> convex_hull(std::vector<Point> &points);
std::vector<Point> convex_hull_parallel(std::vector<Point> &points);
bool is_visible(const Point &p, const Edge &e);
void build_c(Edge &t, const std::vector<Point> &points,
             std::map<random_hull::Edge, std::vector<int>> &C);
void process_ridge(const Edge &t1, const Point &r, const Edge &t2,
                   const std::vector<Point> &points);

void process_ridge_sequential(const Edge &t1, const Point &r, const Edge &t2,
                              const std::vector<Point> &points);

void merge_sets(std::map<random_hull::Edge, std::vector<int>> &C,
                const random_hull::Edge &t1, const random_hull::Edge &t2,
                const random_hull::Edge &t, const std::vector<Point> &points);

Edge join(const Point &p, const Point &r, const Edge &t1);

size_t get_min(const std::vector<int> &points_set);
void process_other_ridge(const Point &r1, const Edge &t,
                         const std::vector<Point> &points);

void process_other_ridge_sequential(
    const Point &r1, const Edge &t,
    const std::vector<Point> &points); // namespace random_hull_testing
                                       //
std::set<random_hull::Edge>
__convex_hull__sequential(const std::vector<Point> &points);
void print_ridge(const random_hull::Edge &t1, const random_hull::Edge &t2,
                 const Point &r);

void print_edge(const random_hull::Edge &t1);

} // namespace random_hull
// namespace random_hull

