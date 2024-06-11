#ifndef MULTIMAP_H
#define MULTIMAP_H
#include <atomic>
#include <points.h>

namespace random_hull {

template <typename Key, typename Tp> class Entry {
  public:
    std::pair<Key, Tp> data;
    std::atomic<bool> taken{false};

    // std::atomic_flag check;

    void set_data(const Key &key, const Tp &value);
    const Key &get_key();
    const Tp &get_value();
};

template <typename Key, typename Tp> class multimap {
  private:
    // Key key;
    // Tp value;
    size_t __capacity;
    Entry<Key, Tp> *__table;

  public:
    multimap() {
        __capacity = 1000000;
        __table = new Entry<Key, Tp>[__capacity];
    }

    ~multimap() { delete __table; }

    multimap(size_t __capacity) : __capacity(__capacity) {
        __table = new Entry<Key, Tp>[__capacity];
    }

    bool insert_and_set(const Key &key, const Tp &value);

    const Tp &get_value(const Key &key, const Tp &not_value);

    size_t increment(size_t i);

    size_t get_entry(const Key &key);
};
template <typename Key, typename Tp>
bool multimap<Key, Tp>::insert_and_set(const Key &key, const Tp &value) {
  
    bool expect = false;

    size_t entry = get_entry(key);

    bool ret_value = true;

    while (!__table[entry].taken.compare_exchange_strong(expect, true)) {
      //std::cout << "C";

        if (__table[entry].get_key() == key) {
            ret_value = false;
        }

        expect = false;
        entry = increment(entry);
    }

    __table[entry].set_data(key, value);

    return ret_value;
}

template <typename Key, typename Tp>
void Entry<Key, Tp>::set_data(const Key &key, const Tp &value) {
    data = std::make_pair(key, value);
}

template <typename Key, typename Tp> const Key &Entry<Key, Tp>::get_key() {
    return data.first;
}

template <typename Key, typename Tp> const Tp &Entry<Key, Tp>::get_value() {
    return data.second;
}

template <typename Key, typename Tp>
size_t multimap<Key, Tp>::get_entry(const Key &key) {
    return std::hash<Key>()(key) % __capacity;
}

template <typename Key, typename Tp>
size_t multimap<Key, Tp>::increment(size_t i) {
    return (i + 1) % __capacity;
}

template <typename Key, typename Tp>
const Tp &multimap<Key, Tp>::get_value(const Key &key, const Tp &not_value) {
    size_t i = get_entry(key);

    while (__table[i].taken.load()) {
        if (__table[i].get_key() == key) {

            if (__table[i].get_value() != not_value) {
                return __table[i].get_value();
            }
        }

        i = increment(i);
    }
    return __table[i].get_value();
}

} // namespace random_hull
#endif
