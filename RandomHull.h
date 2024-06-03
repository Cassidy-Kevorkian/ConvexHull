#include <atomic>
#include <limits>
#include <map>
#include <mutex>
#include <points.h>
#include <set>
#include <vector>
namespace random_hull {
typedef std::pair<Point, Point> Edge;
std::vector<Edge> convex_hull(const std::vector<Point> &points);
bool is_visible(const Point &p, const Edge &e);
void build_C(Edge &t, const std::vector<Point> &points);
void ProcessRidge(const Edge &t1, const Point &r, const Edge &t2);
} // namespace random_hull

