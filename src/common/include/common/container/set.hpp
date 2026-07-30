#ifndef __COMMON__INCLUDE__COMMON__CONTAINER__SET_HPP__
#define __COMMON__INCLUDE__COMMON__CONTAINER__SET_HPP__

#include <functional>
#include <initializer_list>
#include <string>
#include <unordered_set>

#include "iterable.hpp"

/**
 * @brief Default hash function for types that do not have a custom hash
 *        implementation. Uses std::hash<T> as the default.
 *
 * @tparam T The type for which the default hash is defined.
 */
template <typename T>
struct DefaultHash
{
    /// The default hash function type for the given type T
    using type = std::hash<T>;
};

/**
 * @brief Concept to check if a type has a nested Hash type defined.
 *
 * @tparam T The type to check for the presence of a nested Hash type.
 */
template <typename T>
concept HasHash = requires(T type) { typename T::Hash; };

/**
 * @brief Specialization of DefaultHash for types that have a nested Hash type.
 *        Uses the nested Hash type as the default hash function.
 *
 * @tparam T The type for which the specialized default hash is defined.
 */
template <HasHash T>
struct DefaultHash<T>
{
    /// The default hash function type for the given type T, using its nested
    /// Hash type
    using type = typename T::Hash;
};

/**
 * @brief A set container that stores unique elements and provides basic set
 * operations.
 *
 * @tparam T The type of elements stored in the set.
 * @tparam Hash The hash function used for hashing the elements (default is
 * std::hash<T>).
 */
template <typename T, typename Hash = typename DefaultHash<T>::type>
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

    // cppcheck-suppress functionStatic -- false positive
    bool remove(const T& value);
};

/**
 * @brief A set container that stores unique elements and provides basic set
 * operations, specifically for types that have a nested Hash type defined.
 *
 * @tparam T The type of elements stored in the set, which must have a nested
 * Hash type.
 */
template <typename T>
using IdSet = Set<T, typename T::Hash>;

#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__SET_TPP__
#include "set.tpp"
#endif

#endif   // __COMMON__INCLUDE__COMMON__CONTAINER__SET_HPP__