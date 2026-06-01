#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__MAP_TPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__MAP_TPP__

#include "map.hpp"

/**
 * @brief Returns a const reference to the value associated with the specified
 * key.
 *
 * @param key The key of the value to retrieve.
 * @return A const reference to the value associated with the specified key.
 */
template <typename Key, typename Value, typename Hash>
const Value& Map<Key, Value, Hash>::operator[](const Key& key) const
{
    return _items.at(key);
}

template <typename Key, typename Value, typename Hash>
const Value& Map<Key, Value, Hash>::at(const Key& key) const
{
    return _items.at(key);
}

template <typename Key, typename Value, typename Hash>
bool Map<Key, Value, Hash>::add(const Key& key, const Value& value)
{
    auto [_, inserted] = _items.emplace(key, value);
    return inserted;
}

template <typename Key, typename Value, typename Hash>
void Map<Key, Value, Hash>::addUnchecked(const Key& key, const Value& value)
{
    _items.emplace(key, value);
}

template <typename Key, typename Value, typename Hash>
bool Map<Key, Value, Hash>::remove(const Key& key)
{
    return _items.erase(key) > 0;
}

template <typename Key, typename Value, typename Hash>
void Map<Key, Value, Hash>::removeUnchecked(const Key& key)
{
    _items.erase(key);
}

template <typename Key, typename Value, typename Hash>
template <std::ranges::range R>
void Map<Key, Value, Hash>::removeUnchecked(const R& keys)
{
    for (const auto& key : keys)
        _items.erase(key);
}

template <typename Key, typename Value, typename Hash>
bool Map<Key, Value, Hash>::contains(const Key& key) const
{
    return _items.contains(key);
}

template <typename Key, typename Value, typename Hash>
std::unordered_set<Key, Hash> Map<Key, Value, Hash>::getKeys() const
{
    std::unordered_set<Key, Hash> keys;
    for (const auto& [key, _] : _items)
        keys.insert(key);
    return keys;
}

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__MAP_TPP__