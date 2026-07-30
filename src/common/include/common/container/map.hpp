#ifndef __COMMON__INCLUDE__COMMON__CONTAINER__MAP_HPP__
#define __COMMON__INCLUDE__COMMON__CONTAINER__MAP_HPP__

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "iterable.hpp"

/**
 * @brief A map that associates keys with values.
 *
 * @tparam Key The type of the keys in the map.
 * @tparam Value The type of the values in the map.
 * @tparam Hash The type of the hash function to use (default is
 * std::hash<Key>).
 */
template <typename Key, typename Value, typename Hash = std::hash<Key>>
class Map : public Iterable<std::unordered_map<Key, Value, Hash>>
{
   protected:
    using Iterable<std::unordered_map<Key, Value, Hash>>::_items;

   public:
    using Iterable<std::unordered_map<Key, Value, Hash>>::Iterable;

    [[nodiscard]] const Value& operator[](const Key& key) const;
    [[nodiscard]] Value&       operator[](const Key& key);
    [[nodiscard]] const Value& at(const Key& key) const;
    [[nodiscard]] Value&       at(const Key& key);

    [[nodiscard]] bool add(const Key& key, const Value& value);
    void               addUnchecked(const Key& key, const Value& value);

    [[nodiscard]] bool remove(const Key& key);
    void               removeUnchecked(const Key& key);
    template <std::ranges::range R>
    void removeUnchecked(const R& keys);

    [[nodiscard]] bool contains(const Key& key) const;

    [[nodiscard]] std::unordered_set<Key, Hash> getKeys() const;
    [[nodiscard]] std::vector<Value>            getValues() const;

    void clear();
};

#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__MAP_TPP__
#include "map.tpp"
#endif

#endif   // __COMMON__INCLUDE__COMMON__CONTAINER__MAP_HPP__
