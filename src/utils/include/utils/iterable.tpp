#ifndef __UTILS__INCLUDE__UTILS__ITERABLE_TPP__
#define __UTILS__INCLUDE__UTILS__ITERABLE_TPP__

#include <algorithm>
#include <concepts>
#include <unordered_set>
#include <vector>

#include "iterable.hpp"

/**
 * @brief Constructs an Iterable from an existing container.
 *
 * @tparam T The type of elements in the container.
 * @tparam Container The type of the underlying container (default is
 * std::vector<T>).
 * @param items The container to initialize the Iterable with.
 */
template <typename T, typename Container>
Iterable<T, Container>::Iterable(const Container& items) : _items(items)
{
}

/**
 * @brief Returns an iterator to the beginning of the container.
 *
 * @return An iterator to the beginning of the container.
 */
template <typename T, typename Container>
auto Iterable<T, Container>::begin()
{
    return _items.begin();
}

/**
 * @brief Returns an iterator to the end of the container.
 *
 * @return An iterator to the end of the container.
 */
template <typename T, typename Container>
auto Iterable<T, Container>::end()
{
    return _items.end();
}

/**
 * @brief Returns a const iterator to the beginning of the container.
 *
 * @return A const iterator to the beginning of the container.
 */
template <typename T, typename Container>
auto Iterable<T, Container>::begin() const
{
    return _items.begin();
}

/**
 * @brief Returns a const iterator to the end of the container.
 *
 * @return A const iterator to the end of the container.
 */
template <typename T, typename Container>
auto Iterable<T, Container>::end() const
{
    return _items.end();
}

/**
 * @brief Returns a const reference to the item at the specified index.
 *
 * @param index The index of the item to retrieve.
 * @return A const reference to the item at the specified index.
 */
template <typename T, typename Container>
const T& Iterable<T, Container>::operator[](std::size_t index) const
{
    return _items[index];
}

/**
 * @brief Checks if the container is empty.
 *
 * @return true if the container is empty, false otherwise.
 */
template <typename T, typename Container>
bool Iterable<T, Container>::empty() const
{
    return _items.empty();
}

/**
 * @brief Returns the number of items in the container.
 *
 * @return The number of items in the container.
 */
template <typename T, typename Container>
auto Iterable<T, Container>::size() const
{
    return _items.size();
}

/**
 * @brief Sets the items in the container.
 *
 * @param items The container of items to set.
 */
template <typename T, typename Container>
void Iterable<T, Container>::set(const Container& items)
{
    _items = items;
}

/**
 * @brief Returns a reference to the underlying container.
 *
 * @return A reference to the underlying container.
 */
template <typename T, typename Container>
Container& Iterable<T, Container>::getItems()
{
    return _items;
}

/**
 * @brief Returns a const reference to the underlying container.
 *
 * @return A const reference to the underlying container.
 */
template <typename T, typename Container>
const Container& Iterable<T, Container>::getItems() const
{
    return _items;
}

/**
 * @brief Adds an item to the container.
 *
 * @param item The item to add.
 */
template <typename T, typename Container>
void Iterable<T, Container>::add(const T& item)
requires std::same_as<Container, std::vector<T>>
{
    _items.push_back(item);
}

/**
 * @brief Adds a range of items to the container.
 *
 * @param items The range of items to add.
 */
template <typename T, typename Container>
void Iterable<T, Container>::add(const Container& items)
requires std::same_as<Container, std::vector<T>>
{
    _items.insert(_items.end(), items.begin(), items.end());
}

/**
 * @brief Removes an item from the container.
 *
 * @param id The id of the item to remove.
 * @return true if the item was removed, false otherwise.
 */
template <typename T, typename Container>
template <typename IdType>
requires HasId<T, IdType>
bool Iterable<T, Container>::remove(IdType id)
{
    auto it = std::ranges::find_if(
        _items,
        [id](const T& type) { return type.getId() == id; }
    );

    if (it != _items.end())
    {
        _items.erase(it);
        return true;
    }
    return false;
}

/**
 * @brief Removes a range of items from the container.
 *
 * @param ids The range of ids of the items to remove.
 */
template <typename T, typename Container>
template <typename IdContainer>
requires HasIdRange<IdContainer, T>
void Iterable<T, Container>::remove(const IdContainer& ids)
{
    std::erase_if(
        _items,
        [&](const T& item) { return std::ranges::contains(ids, item.getId()); }
    );
}

/**
 * @brief Checks if an item with the specified id exists in the container.
 *
 * @param id The id of the item to check for.
 * @return true if an item with the specified id exists, false otherwise.
 */
template <typename T, typename Container>
template <typename IdType>
requires HasId<T, IdType>
bool Iterable<T, Container>::contains(IdType id) const
{
    return std::ranges::any_of(
        _items,
        [id](const T& type) { return type.getId() == id; }
    );
}

/**
 * @brief Returns a set of all item IDs in the container.
 *
 * @return A set of all item IDs in the container.
 */
template <typename T, typename Container>
template <typename IdType, typename Hash>
std::unordered_set<IdType, Hash> Iterable<T, Container>::_getIds() const
{
    std::unordered_set<IdType, Hash> idSet;
    for (const auto& item : _items)
        idSet.insert(item.getId());

    return idSet;
}

#endif   // __UTILS__INCLUDE__UTILS__ITERABLE_TPP__