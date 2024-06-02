#include <RandomHull.h>
#include <utility>

std::vector<random_hull::Facet>
random_hull::convex_hull(const std::vector<Point> &points) {
    return {};
};

template <typename Key, typename Tp>
bool random_hull::multimap<Key, Tp>::insert_and_set(const Key &key,
                                                    const Tp &value) {
    bool expect = false;

    size_t entry = get_entry(key);

    while(!__table[entry].taken.compare_exchange_strong(expect, true)) {
        expect = false;
        entry = increment(entry);
    }

    __table[entry].set_data(key, value);

}

template <typename Key, typename Tp>
void random_hull::Entry<Key, Tp>::set_data(const Key &key, const Tp &value) {
    data = std::make_pair(key, value);
}


template <typename Key, typename Tp>
size_t random_hull::multimap<Key, Tp>::get_entry(const Key &key) {
    return std::hash<Key>()(key);
}

template <typename Key, typename Tp>
size_t random_hull::multimap<Key, Tp>::increment(size_t i) {
    return (i + 1) % __capacity;
}

