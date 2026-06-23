#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__SET_TPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__SET_TPP__

#include "set.hpp"

template <typename T, typename Hash>
Set<T, Hash>::Set(std::initializer_list<T> initList) : IterableBase(initList)
{
}

template <typename T, typename Hash>
bool Set<T, Hash>::contains(const T& value) const
{
    return IterableBase::_items.find(value) != IterableBase::_items.end();
}

template <typename T, typename Hash>
void Set<T, Hash>::insert(const T& value)
{
    IterableBase::_items.insert(value);
}

template <typename T, typename Hash>
void Set<T, Hash>::combine(const Set<T, Hash>& other)
{
    for (const auto& value : other)
        insert(value);
}

template <typename T, typename Hash>
Set<T, Hash> Set<T, Hash>::operator&(const Set<T, Hash>& other) const
{
    Set<T, Hash> result;
    for (const auto& value : IterableBase::_items)
    {
        if (other.contains(value))
            result.insert(value);
    }
    return result;
}

template <typename T, typename Hash>
bool Set<T, Hash>::intersects(const Set<T, Hash>& other) const
{
    return !(this->operator&(other)).empty();
}

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__SET_TPP__