#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_TPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_TPP__

#include "id_map.hpp"

template <typename Value>
requires HasId<Value>
template <std::ranges::range R>
void IdMap<Value>::setUnchecked(R&& values)
{
    addUnchecked(std::forward<R>(values));
}

template <typename Value>
requires HasId<Value>
bool IdMap<Value>::add(const Value& value)
{
    return Base::add(value.getId(), value);
}

template <typename Value>
requires HasId<Value>
template <std::ranges::range R>
void IdMap<Value>::addUnchecked(R&& values)
{
    for (const auto& value : std::forward<R>(values))
        Base::addUnchecked(value.getId(), value);
}

template <typename Value>
requires HasId<Value>
void IdMap<Value>::addUnchecked(const Value& value)
{
    Base::addUnchecked(value.getId(), value);
}

template <typename Value>
requires HasId<Value>
auto IdMap<Value>::getIds() const -> std::unordered_set<Key, typename Key::Hash>
{
    return Base::getKeys();
}

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_TPP__