#ifndef __COMMON__INCLUDE__COMMON__CONTAINER__SET_HPP__
#define __COMMON__INCLUDE__COMMON__CONTAINER__SET_HPP__

#include <functional>
#include <initializer_list>
#include <string>
#include <unordered_set>

#include "iterable.hpp"

/**
 * @brief A set container that stores unique elements and provides basic set
 * operations.
 *
 * @tparam T The type of elements stored in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 */
template <typename T, typename Hash = std::hash<T>>
class Set : public Iterable<std::unordered_set<T, Hash>>
{
   private:
    /// The base class type
    using IterableBase = Iterable<std::unordered_set<T, Hash>>;

   public:
    using Iterable<std::unordered_set<T, Hash>>::Iterable;

    Set(std::initializer_list<T> initList);

    // cppcheck-suppress functionStatic -- false positive
    [[nodiscard]] bool contains(const T& value) const;

    // cppcheck-suppress functionStatic -- false positive
    void insert(const T& value);

    void combine(const Set<T, Hash>& other);

    Set operator&(const Set<T, Hash>& other) const;
    Set operator-(const Set<T, Hash>& other) const;

    // cppcheck-suppress functionStatic -- false positive
    const T& front() const;

    bool operator==(const Set<T, Hash>& other) const;

    bool intersects(const Set<T, Hash>& other) const;

    // cppcheck-suppress functionStatic -- false positive
    [[nodiscard]] std::string toString() const;

    template <std::ranges::range R, typename F>
    static Set<T, Hash> fromRange(const R& range, F&& func);
};

template <typename T>
using IdSet = Set<T, typename T::Hash>;

#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__SET_TPP__
#include "set.tpp"
#endif

#endif   // __COMMON__INCLUDE__COMMON__CONTAINER__SET_HPP__