#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__VECTOR_TPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__VECTOR_TPP__

#include "vector.hpp"

/**
 * @brief Returns a const reference to the item at the specified index.
 *
 * @param index The index of the item to retrieve.
 * @return A const reference to the item at the specified index.
 */
template <typename T>
const T& Vector<T>::operator[](std::size_t index) const
{
    return _items[index];
}

/**
 * @brief Adds an item to the container.
 *
 * @param item The item to add.
 */
template <typename T>
void Vector<T>::add(const T& item)
{
    _items.push_back(item);
}

/**
 * @brief Adds a range of items to the container.
 *
 * @param items The range of items to add.
 */
template <typename T>
void Vector<T>::add(const std::vector<T>& items)
{
    _items.insert(_items.end(), items.begin(), items.end());
}

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__VECTOR_TPP__