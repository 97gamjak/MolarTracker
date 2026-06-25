#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_TPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_TPP__

#include "id_map.hpp"

/**
 * @brief Sets the values in the map based on a range of values with IDs.
 *
 * @details This method takes a range of values that have IDs and adds them to
 * the map, using the IDs as keys. It uses the addUnchecked method to add the
 * values to the map without checking for duplicates, so it should be used with
 * caution.
 *
 * @tparam Value
 * @param values
 */
template <typename Value>
requires HasId<Value>
template <std::ranges::range R>
void IdObjectMap<Value>::setUnchecked(R&& values)
{
    addUnchecked(std::forward<R>(values));
}

/**
 * @brief Adds a value to the map based on its ID.
 *
 * @details This method takes a value that has an ID and adds it to the map,
 * using the ID as the key. It checks for duplicates and returns false if a
 * value with the same ID already exists in the map, and true if the value was
 * successfully added.
 *
 * @tparam Value
 * @param value
 * @return true if the value was added, false if a value with the same ID
 * already exists in the map.
 */
template <typename Value>
requires HasId<Value>
bool IdObjectMap<Value>::add(const Value& value)
{
    return Base::add(extractId(value), value);
}

/**
 * @brief Adds a range of values to the map based on their IDs without checking
 * for duplicates.
 *
 * @tparam Value
 * @param values A range of values to add to the map.
 */
template <typename Value>
requires HasId<Value>
template <std::ranges::range R>
void IdObjectMap<Value>::addUnchecked(R&& values)
{
    if constexpr (std::same_as<std::decay_t<R>, IdObjectMap<Value>>)
    {
        // Direct copy from another IdObjectMap — already has IDs
        for (const auto& [key, value] : values)
            Base::addUnchecked(key, value);
    }
    else
    {
        // Range of bare values — extract IDs
        for (const auto& value : std::forward<R>(values))
            Base::addUnchecked(extractId(value), value);
    }
}

/**
 * @brief Adds a value to the map based on its ID without checking for
 * duplicates.
 *
 * @tparam Value
 * @param value
 */
template <typename Value>
requires HasId<Value>
void IdObjectMap<Value>::addUnchecked(const Value& value)
{
    Base::addUnchecked(extractId(value), value);
}

/**
 * @brief Gets the set of IDs currently in the map.
 *
 * @details This method returns a set of all the IDs that are currently keys in
 * the map. It uses the getKeys method from the base Map class to retrieve the
 * keys and returns them as an unordered_set.
 *
 * @tparam Key
 * @tparam Value
 * @return A set of all the IDs currently in the map.
 */
template <IsId Key, typename Value>
auto IdMap<Key, Value>::getIds() const
    -> std::unordered_set<Key, typename Key::Hash>
{
    return Base::getKeys();
}

template <typename Value>
requires HasId<Value>
template <std::ranges::range R>
IdObjectMap<Value>::IdObjectMap(R&& values)
{
    for (const auto& elem : std::forward<R>(values))
    {
        if constexpr (requires { elem.second; })
        {
            // Range of pairs (from iterating IdObjectMap)
            Base::addUnchecked(extractId(elem.second), elem.second);
        }
        else
        {
            // Range of values
            Base::addUnchecked(extractId(elem), elem);
        }
    }
}

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_TPP__