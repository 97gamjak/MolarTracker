#ifndef __COMMON__INCLUDE__COMMON__CONTAINER__ID_ID_MAP_TPP__
#define __COMMON__INCLUDE__COMMON__CONTAINER__ID_ID_MAP_TPP__

#include "id_id_map.hpp"

/**
 * @brief Combines another IdIdMap into this one. This is used to merge ID
 * remappings from multiple operations, allowing for the consolidation of ID
 * mappings from different sources into a single map, which can be useful for
 * maintaining a comprehensive mapping of IDs across various operations or
 * transactions that may involve multiple steps or changes to IDs.
 *
 * @tparam IdType
 * @tparam SecondIdType
 * @param other The other IdIdMap to combine with this one.
 * @return true if the maps were successfully combined without conflicts, false
 * if there were conflicting mappings (i.e., the same key maps to different
 * values in the two maps).
 */
template <typename IdType, typename SecondIdType>
bool IdIdMap<IdType, SecondIdType>::combine(const IdIdMap& other)
{
    for (const auto& [key, value] : other)
    {
        if (!Base::add(key, value))
            return false;
    }

    return true;
}

/**
 * @brief Converts the IdIdMap to a string representation for debugging
 * purposes. This method iterates through the map and constructs a string that
 * represents the key-value pairs in the map, which can be useful for logging
 * and debugging to understand the current state of the ID mappings.
 *
 * @tparam IdType
 * @tparam SecondIdType
 * @return std::string A string representation of the IdIdMap, showing the
 * mappings of keys to values in a readable format.
 */
template <typename IdType, typename SecondIdType>
std::string IdIdMap<IdType, SecondIdType>::toString() const
{
    std::string result = "{";
    for (const auto& [key, value] : *this)
    {
        result += key.toString() + ": " + value.toString() + ", ";
    }
    if (result.size() > 1)
        result.pop_back(), result.pop_back();   // Remove trailing ", "
    result += "}";
    return result;
}

#endif   // __COMMON__INCLUDE__COMMON__CONTAINER__ID_ID_MAP_TPP__