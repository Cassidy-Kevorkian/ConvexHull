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
} // namespace random_hull
