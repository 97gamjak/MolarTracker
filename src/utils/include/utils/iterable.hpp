#ifndef __UTILS__INCLUDE__UTILS__ITERABLE_HPP__
#define __UTILS__INCLUDE__UTILS__ITERABLE_HPP__

#include <unordered_set>
#include <vector>

/**
 * @brief A container that provides iterable functionality.
 *
 * @tparam T The type of elements in the container.
 * @tparam Container The type of the underlying container (default is
 * std::vector<T>).
 */
template <typename T, typename Container = std::vector<T>>
class Iterable
{
   private:
    /// The underlying container of items
    Container _items;

   public:
    Iterable() = default;
    explicit Iterable(const Container& items);

    auto begin();
    auto end();
    auto begin() const;
    auto end() const;

    void set(const Container& items);

    Container&       getItems();
    const Container& getItems() const;

    void add(const T& item)
    requires std::same_as<Container, std::vector<T>>;

    void add(const Container& items)
    requires std::same_as<Container, std::vector<T>>;

    template <typename IdType>
    requires requires(T type) {
        { type.getId() } -> std::same_as<IdType>;
    }
    bool remove(IdType id);

    template <typename IdContainer>
    requires std::ranges::range<IdContainer> && requires(T type) {
        {
            type.getId()
        } -> std::same_as<std::ranges::range_value_t<IdContainer>>;
    }
    void remove(const IdContainer& ids);

   protected:
    template <typename IdType, typename Hash = std::hash<IdType>>
    [[nodiscard]] std::unordered_set<IdType, Hash> _getIds() const;
};

#ifndef __UTILS__INCLUDE__UTILS__ITERABLE_TPP__
#include "iterable.tpp"
#endif

#endif   // __UTILS__INCLUDE__UTILS__ITERABLE_HPP__