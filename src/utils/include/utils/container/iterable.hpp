#ifndef __UTILS__INCLUDE__UTILS__ITERABLE_HPP__
#define __UTILS__INCLUDE__UTILS__ITERABLE_HPP__

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
    explicit Iterable(const Container& items);

    auto begin();
    auto end();
    auto begin() const;
    auto end() const;

    [[nodiscard]] bool empty() const;
    [[nodiscard]] auto size() const;

   protected:
    [[nodiscard]] Container&       getItems();
    [[nodiscard]] const Container& getItems() const;

    template <typename IdType, typename Hash = std::hash<IdType>>
    [[nodiscard]]
    std::unordered_set<IdType, Hash> _getIds() const;
};

#ifndef __UTILS__INCLUDE__UTILS__ITERABLE_TPP__
#include "iterable.tpp"
#endif

#endif   // __UTILS__INCLUDE__UTILS__ITERABLE_HPP__