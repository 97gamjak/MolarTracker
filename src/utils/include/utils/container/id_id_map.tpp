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

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__ID_ID_MAP_TPP__