#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__VECTOR_TPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__VECTOR_TPP__

#include "vector.hpp"

/**
 * @brief Construct a new Vector< T>:: Vector object
 *
 * @tparam T
 * @param items
 */
template <typename T>
Vector<T>::Vector(std::initializer_list<T> items)
    : Iterable<std::vector<T>>(items)
{
}

/**
 * @brief Construct a new Vector< T>:: Vector object from a range of items
 *
 * @tparam T
 * @tparam R
 * @param range
 */
template <typename T>
template <std::ranges::input_range R>
requires std::convertible_to<std::ranges::range_value_t<R>, T>
// NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
Vector<T>::Vector(R&& range)
    : Iterable<std::vector<T>>(
          std::vector<T>(std::ranges::begin(range), std::ranges::end(range))
      )
{
}

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
 * @brief Returns a reference to the first item in the container.
 *
 * @return A reference to the first item in the container.
 */
template <typename T>
auto Vector<T>::front() const
{
    return _items.front();
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