#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__MAP_HPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__MAP_HPP__

#include <unordered_map>

#include "iterable.hpp"

template <typename T>
concept HasId = requires(T item) {
    { item.getId() };
};

template <typename T>
using IdOf = decltype(std::declval<T>().getId());

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class Map : public Iterable<std::unordered_map<Key, Value, Hash>>
{
   protected:
    using Iterable<std::unordered_map<Key, Value, Hash>>::_items;

   public:
    using Iterable<std::unordered_map<Key, Value, Hash>>::Iterable;

    [[nodiscard]] const Value& operator[](const Key& key) const;
    [[nodiscard]] const Value& at(const Key& key) const;

    [[nodiscard]] bool add(const Key& key, const Value& value);
    void               addUnchecked(const Key& key, const Value& value);

    [[nodiscard]] bool remove(const Key& key);
    void               removeUnchecked(const Key& key);
    template <std::ranges::range R>
    void removeUnchecked(const R& keys);

    [[nodiscard]] bool contains(const Key& key) const;

    [[nodiscard]] std::unordered_set<Key, Hash> getKeys() const;
};

#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__MAP_TPP__
#include "map.tpp"
#endif

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__MAP_HPP__
