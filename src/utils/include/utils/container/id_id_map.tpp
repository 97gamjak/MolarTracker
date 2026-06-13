#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ID_ID_MAP_TPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__ID_ID_MAP_TPP__

#include "id_id_map.hpp"

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

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__ID_ID_MAP_TPP__