#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ITERABLE_HPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__ITERABLE_HPP__

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
    // cppcheck-suppress noExplicitConstructor
    Iterable(const Container& items);
    // cppcheck-suppress noExplicitConstructor
    Iterable(Container&& items);

    /// @cond DOXYGEN_IGNORE
    Iterable(const Iterable& other)                = default;
    Iterable& operator=(const Iterable& other)     = default;
    Iterable(Iterable&& other) noexcept            = default;
    Iterable& operator=(Iterable&& other) noexcept = default;
    ~Iterable()                                    = default;
    /// @endcond
    // NOLINTEND(google-explicit-constructor, hicpp-explicit-conversions)

    auto begin();
    auto end();
    auto begin() const;
    auto end() const;

    [[nodiscard]] bool empty() const;
    [[nodiscard]] auto size() const;

    [[nodiscard]] Container&       getItems();
    [[nodiscard]] const Container& getItems() const;

    template <typename Predicate>
    auto erase_if(Predicate&& predicate);
};

#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ITERABLE_TPP__
#include "iterable.tpp"
#endif

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__ITERABLE_HPP__