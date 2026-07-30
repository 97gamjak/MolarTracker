#ifndef __COMMON__INCLUDE__COMMON__CONTAINER__SET_TPP__
#define __COMMON__INCLUDE__COMMON__CONTAINER__SET_TPP__

#include "exceptions/out_of_range.hpp"
#include "set.hpp"

/**
 * @brief Constructs a Set from an initializer list.
 *
 * @tparam T The type of elements in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 * @param initList The initializer list to initialize the Set with.
 */
template <typename T, typename Hash>
Set<T, Hash>::Set(std::initializer_list<T> initList) : IterableBase(initList)
{
}

/**
 * @brief Checks if the set contains a specific value.
 *
 * @tparam T The type of elements in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 * @param value The value to check for in the set.
 * @return true if the set contains the value, false otherwise.
 */
template <typename T, typename Hash>
bool Set<T, Hash>::contains(const T& value) const
{
    return IterableBase::_items.find(value) != IterableBase::_items.end();
}

/**
 * @brief Inserts a value into the set.
 *
 * @tparam T The type of elements in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 * @param value The value to insert into the set.
 */
template <typename T, typename Hash>
void Set<T, Hash>::insert(const T& value)
{
    IterableBase::_items.insert(value);
}

/**
 * @brief Combines the current set with another set, adding all elements from
 * the other set to the current set.
 *
 * @tparam T The type of elements in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 * @param other The other set to combine with the current set.
 */
template <typename T, typename Hash>
void Set<T, Hash>::combine(const Set<T, Hash>& other)
{
    for (const auto& value : other)
        insert(value);
}

/**
 * @brief Returns a new set that is the intersection of the current set and
 * another set.
 *
 * @tparam T The type of elements in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 * @param other The other set to intersect with the current set.
 * @return A new set containing elements that are present in both sets.
 */
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

/**
 * @brief Returns a new set that is the difference of the current set and
 * another set.
 *
 * @tparam T The type of elements in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 * @param other The other set to subtract from the current set.
 * @return A new set containing elements that are present in the current set
 * but not in the other set.
 */
template <typename T, typename Hash>
Set<T, Hash> Set<T, Hash>::operator-(const Set<T, Hash>& other) const
{
    Set<T, Hash> result;
    for (const auto& value : IterableBase::_items)
    {
        if (!other.contains(value))
            result.insert(value);
    }
    return result;
}

/**
 * @brief Checks if the current set is equal to another set.
 *
 * @tparam T The type of elements in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 * @param other The other set to compare with the current set.
 * @return true if both sets contain the same elements, false otherwise.
 */
template <typename T, typename Hash>
bool Set<T, Hash>::operator==(const Set<T, Hash>& other) const
{
    return this->operator-(other).empty() && other.operator-(*this).empty();
}

/**
 * @brief Checks if the current set intersects with another set, meaning they
 * share at least one common element.
 *
 * @tparam T The type of elements in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 * @param other The other set to check for intersection with the current set.
 * @return true if the sets intersect, false otherwise.
 */
template <typename T, typename Hash>
bool Set<T, Hash>::intersects(const Set<T, Hash>& other) const
{
    return !(this->operator&(other)).empty();
}

/**
 * @brief Returns the first element in the set.
 *
 * @tparam T The type of elements in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 * @return A const reference to the first element in the set.
 * @throws OutOfRange if the set is empty.
 */
template <typename T, typename Hash>
const T& Set<T, Hash>::front() const
{
    if (IterableBase::_items.empty())
        throw OutOfRange("Set is empty");

    return *(IterableBase::_items.begin());
}

/**
 * @brief Returns a string representation of the set.
 *
 * @tparam T The type of elements in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 * @return A string representation of the set.
 */
template <typename T, typename Hash>
std::string Set<T, Hash>::toString() const
{
    std::string result = "{";
    for (const auto& value : IterableBase::_items)
    {
        // check if toString is available for the type T, if it is, use it,
        // otherwise use std::to_string
        if constexpr (std::is_invocable_v<decltype(&T::toString), T>)
            result += value.toString() + ", ";
        else
            result += std::to_string(value) + ", ";
    }
    if (!IterableBase::_items.empty())
        result.pop_back(),
            result.pop_back();   // Remove trailing comma and space
    result += "}";
    return result;
}

/**
 * @brief Creates a Set from a range of values, applying a transformation
 * function to each value.
 *
 * @tparam T The type of elements in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 * @tparam R The type of the range of values.
 * @tparam F The type of the transformation function.
 * @param range The range of values to transform and add to the set.
 * @param func The transformation function to apply to each value in the range.
 * @return A new Set containing the transformed values.
 */
template <typename T, typename Hash>
template <std::ranges::range R, typename F>
Set<T, Hash> Set<T, Hash>::fromRange(const R& range, F&& func)
{
    Set<T, Hash> result;
    for (const auto& item : range)
        result.insert(std::forward<F>(func)(item));

    return result;
}

/**
 * @brief Removes a value from the set.
 *
 * @tparam T The type of elements in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 * @param value The value to remove from the set.
 * @return true if the value was removed, false if it was not found in the set.
 */
template <typename T, typename Hash>
bool Set<T, Hash>::remove(const T& value)
{
    return IterableBase::_items.erase(value) > 0;
}

#endif   // __COMMON__INCLUDE__COMMON__CONTAINER__SET_TPP__