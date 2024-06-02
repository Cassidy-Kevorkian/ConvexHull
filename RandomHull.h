#include <atomic>
#include <limits>
#include <map>
#include <points.h>
#include <vector>
namespace random_hull {

typedef std::pair<Point, Point> Facet;

std::vector<Facet> convex_hull(const std::vector<Point> &points);

template <typename Key, typename Tp> class Entry {
  public:
    std::pair<Key, Tp> data;
    std::atomic<bool> taken{false};

    //std::atomic_flag check;

    void set_data(const Key &key, const Tp &value);
    const Key& get_key();
    const Tp& get_value();
};

template <typename Key, typename Tp> class multimap {
  private:
    Key key;
    Tp value;
    size_t __capacity;
    Entry<Key, Tp> *__table;

  public:
    multimap() {
        __capacity = 4000000;
        __table = new Entry<Key, Tp>[__capacity];
    }

    multimap(size_t __capacity) : __capacity(__capacity) {
        __table = new Entry<Key, Tp>[__capacity];
    }

    bool insert_and_set(const Key &key, const Tp &value);

    Tp &get_value(const Key &key, const Tp &not_value);

    size_t increment(size_t i);

    size_t get_entry(const Key &key);
};
} // namespace random_hull
