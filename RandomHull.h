#include <atomic>
#include <limits>
#include <map>
#include <points.h>
#include <vector>
namespace random_hull {

typedef std::pair<Point, Point> Facet;

std::vector<Facet> convex_hull(const std::vector<Point> &points);

template <typename Key, typename Tp> class Entry {
  private:
    std::pair<Key, Tp> __data;
    std::atomic<bool> taken;
    std::atomic_flag check;
};

template <typename Key, typename Tp>
class multimap {
  private:
    Key key;

    Tp value;
    size_t __capacity;

  public:
    multimap() { __capacity = 4000000; }
    multimap(std::uint64_t __capacity) : __capacity(__capacity) {}

    bool insert_and_set(const Key &key, const Tp &value);

    Tp &get_value(const Key &key, const Tp &not_value);
};
} // namespace random_hull
