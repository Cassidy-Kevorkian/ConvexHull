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

    bool ret_value = true;

    while(!__table[entry].taken.compare_exchange_strong(expect, true)) {

        if(__table[entry].get_key() == key) {
            ret_value = false;
        }

        expect = false;
        entry = increment(entry);
    }

    __table[entry].set_data(key, value);

    return ret_value;

}

template <typename Key, typename Tp>
void random_hull::Entry<Key, Tp>::set_data(const Key &key, const Tp &value) {
    data = std::make_pair(key, value);
}

template <typename Key, typename Tp>
const Key& random_hull::Entry<Key, Tp>::get_key() {
    return data.first;
}

template <typename Key, typename Tp>
const Tp& random_hull::Entry<Key, Tp>::get_value() {
    return data.second;
}


template <typename Key, typename Tp>
size_t random_hull::multimap<Key, Tp>::get_entry(const Key &key) {
    return std::hash<Key>()(key);
}

template <typename Key, typename Tp>
size_t random_hull::multimap<Key, Tp>::increment(size_t i) {
    return (i + 1) % __capacity;
}

template <typename Key, typename Tp>
 Tp &get_value(const Key &key) {
    size_t i = get_entry(Key);
    while(__table[i].taken.load()){
        if(__table[i].get_key() == key){
            return __table[i].get_value();
        }
        i = increment(i);
    }
    return __table[i].get_value();
}

