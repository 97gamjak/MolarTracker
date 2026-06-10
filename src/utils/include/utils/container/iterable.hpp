#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ITERABLE_HPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__ITERABLE_HPP__

#include <unordered_set>

/**
 * @brief A container that provides iterable functionality.
 *
 * @tparam T The type of elements in the container.
 * @tparam Container The type of the underlying container
 */
template <typename Container>
class Iterable
{
   protected:
    /// The underlying container of items
    /// NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
    Container _items;

   public:
    Iterable() = default;
    // NOLINTBEGIN(google-explicit-constructor, hicpp-explicit-conversions)
    Iterable(const Container& items);
    Iterable(Container&& items);
    // NOLINTEND(google-explicit-constructor, hicpp-explicit-conversions)

    auto begin();
    auto end();
    auto begin() const;
    auto end() const;

    [[nodiscard]] bool empty() const;
    [[nodiscard]] auto size() const;

   protected:
    [[nodiscard]] Container&       getItems();
    [[nodiscard]] const Container& getItems() const;
};

#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ITERABLE_TPP__
#include "iterable.tpp"
#endif

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__ITERABLE_HPP__