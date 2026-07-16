#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ITERABLE_TPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__ITERABLE_TPP__

#include <ranges>
#include <utility>

#include "iterable.hpp"

/**
 * @brief Constructs an Iterable from an existing container.
 *
 * @tparam T The type of elements in the container.
 * @tparam Container The type of the underlying container (default is
 * std::vector<T>).
 * @param items The container to initialize the Iterable with.
 */
template <typename Container>
Iterable<Container>::Iterable(const Container& items) : _items(items)
{
}

/**
 * @brief Constructs an Iterable from an rvalue container.
 *
 * @tparam T The type of elements in the container.
 * @tparam Container The type of the underlying container (default is
 * std::vector<T>).
 * @param items The container to initialize the Iterable with.
 */
template <typename Container>
Iterable<Container>::Iterable(Container&& items) : _items(std::move(items))
{
}

/**
 * @brief Returns an iterator to the beginning of the container.
 *
 * @return An iterator to the beginning of the container.
 */
template <typename Container>
auto Iterable<Container>::begin()
{
    return _items.begin();
}

/**
 * @brief Returns an iterator to the end of the container.
 *
 * @return An iterator to the end of the container.
 */
template <typename Container>
auto Iterable<Container>::end()
{
    return _items.end();
}

/**
 * @brief Returns a const iterator to the beginning of the container.
 *
 * @return A const iterator to the beginning of the container.
 */
template <typename Container>
auto Iterable<Container>::begin() const
{
    return _items.begin();
}

/**
 * @brief Returns a const iterator to the end of the container.
 *
 * @return A const iterator to the end of the container.
 */
template <typename Container>
auto Iterable<Container>::end() const
{
    return _items.end();
}

/**
 * @brief Checks if the container is empty.
 *
 * @return true if the container is empty, false otherwise.
 */
template <typename Container>
bool Iterable<Container>::empty() const
{
    return _items.empty();
}

/**
 * @brief Returns the number of items in the container.
 *
 * @return The number of items in the container.
 */
template <typename Container>
auto Iterable<Container>::size() const
{
    return _items.size();
}

/**
 * @brief Returns a reference to the underlying container.
 *
 * @return A reference to the underlying container.
 */
template <typename Container>
Container& Iterable<Container>::getItems()
{
    return _items;
}

/**
 * @brief Returns a const reference to the underlying container.
 *
 * @return A const reference to the underlying container.
 */
template <typename Container>
const Container& Iterable<Container>::getItems() const
{
    return _items;
}

/**
 * @brief Erases elements from the container that satisfy the given predicate.
 *
 * @tparam Predicate The type of the predicate function.
 * @param predicate The predicate function to determine which elements to
 * erase.
 * @return The number of elements erased.
 */
template <typename Container>
template <typename Predicate>
auto Iterable<Container>::erase_if(Predicate&& predicate)
{
    return std::erase_if(_items, std::forward<Predicate>(predicate));
}

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__ITERABLE_TPP__