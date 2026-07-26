#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__MAP_TPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__MAP_TPP__

#include <vector>

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

/**
 * @brief Returns a reference to the value associated with the specified key.
 *
 * @param key The key of the value to retrieve.
 * @return A reference to the value associated with the specified key.
 */
template <typename Key, typename Value, typename Hash>
Value& Map<Key, Value, Hash>::operator[](const Key& key)
{
    return _items[key];
}

/**
 * @brief Returns a const reference to the value associated with the specified
 * key.
 *
 * @param key The key of the value to retrieve.
 * @return A const reference to the value associated with the specified key.
 */
template <typename Key, typename Value, typename Hash>
const Value& Map<Key, Value, Hash>::at(const Key& key) const
{
    return _items.at(key);
}

/**
 * @brief Returns a reference to the value associated with the specified
 * key.
 *
 * @param key The key of the value to retrieve.
 * @return A reference to the value associated with the specified key.
 */
template <typename Key, typename Value, typename Hash>
Value& Map<Key, Value, Hash>::at(const Key& key)
{
    return _items.at(key);
}

/**
 * @brief Adds a key-value pair to the map if the key does not already exist.
 *
 * @param key The key to add.
 * @param value The value to associate with the key.
 * @return true if the key-value pair was added, false if the key already
 * exists in the map.
 */
template <typename Key, typename Value, typename Hash>
bool Map<Key, Value, Hash>::add(const Key& key, const Value& value)
{
    auto [_, inserted] = _items.emplace(key, value);
    return inserted;
}

/**
 * @brief Adds a key-value pair to the map without checking for duplicates.
 *
 * @param key The key to add.
 * @param value The value to associate with the key.
 */
template <typename Key, typename Value, typename Hash>
void Map<Key, Value, Hash>::addUnchecked(const Key& key, const Value& value)
{
    _items.emplace(key, value);
}

/**
 * @brief Removes a key-value pair from the map based on the key.
 *
 * @param key The key of the key-value pair to remove.
 * @return true if the key-value pair was removed, false if the key does not
 * exist in the map.
 */
template <typename Key, typename Value, typename Hash>
bool Map<Key, Value, Hash>::remove(const Key& key)
{
    return _items.erase(key) > 0;
}

/**
 * @brief Removes a key-value pair from the map based on the key without
 * checking if the key exists.
 *
 * @param key The key of the key-value pair to remove.
 */
template <typename Key, typename Value, typename Hash>
void Map<Key, Value, Hash>::removeUnchecked(const Key& key)
{
    _items.erase(key);
}

/**
 * @brief Removes multiple key-value pairs from the map based on a range of
 * keys without checking if the keys exist.
 *
 * @param keys A range of keys corresponding to the key-value pairs to remove.
 */
template <typename Key, typename Value, typename Hash>
template <std::ranges::range R>
void Map<Key, Value, Hash>::removeUnchecked(const R& keys)
{
    for (const auto& key : keys)
        _items.erase(key);
}

/**
 * @brief Checks if the map contains a key.
 *
 * @param key The key to check for.
 * @return true if the map contains the key, false otherwise.
 */
template <typename Key, typename Value, typename Hash>
bool Map<Key, Value, Hash>::contains(const Key& key) const
{
    return _items.contains(key);
}

/**
 * @brief Gets the set of keys currently in the map.
 *
 * @details This method returns a set of all the keys that are currently keys in
 * the map. It iterates through the items in the map and collects the keys into
 * an unordered_set, which is then returned.
 *
 * @return A set of all the keys currently in the map.
 */
template <typename Key, typename Value, typename Hash>
std::unordered_set<Key, Hash> Map<Key, Value, Hash>::getKeys() const
{
    std::unordered_set<Key, Hash> keys;
    for (const auto& [key, _] : _items)
        keys.insert(key);
    return keys;
}

/**
 * @brief Gets a vector of all the values currently in the map.
 *
 * @details This method iterates through the items in the map and collects the
 * values into a vector, which is then returned. This provides an easy way to
 * access all the values in the map without needing to know the keys.
 *
 * @return A vector of all the values currently in the map.
 */
template <typename Key, typename Value, typename Hash>
std::vector<Value> Map<Key, Value, Hash>::getValues() const
{
    std::vector<Value> values;

    for (const auto& [_, value] : _items)
        values.push_back(value);

    return values;
}

/**
 * @brief Clears all key-value pairs from the map.
 *
 * @details This method removes all entries from the map, effectively resetting
 * it to an empty state. After calling this method, the map will contain no
 * keys or values.
 */
template <typename Key, typename Value, typename Hash>
void Map<Key, Value, Hash>::clear()
{
    _items.clear();
}

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__MAP_TPP__