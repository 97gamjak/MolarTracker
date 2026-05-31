#ifndef __UTILS__INCLUDE__UTILS__ITERABLE_HPP__
#define __UTILS__INCLUDE__UTILS__ITERABLE_HPP__

#include <ranges>
#include <unordered_set>
#include <vector>

/**
 * @brief concept to check if a type has a getId() method that returns a
 * specific type.
 *
 * @tparam T
 * @tparam IdType
 */
template <typename T, typename IdType>
concept HasId = requires(T type) {
    { type.getId() } -> std::same_as<IdType>;
};

/**
 * @brief concept to check if a type has a getId() method that returns a type
 * that is the same as the value type of a given range.
 *
 * @tparam IdContainer
 * @tparam T
 * @tparam IdType
 */
template <typename IdContainer, typename T>
concept HasIdRange = requires(IdContainer ids, T type) {
    { type.getId() } -> std::same_as<std::ranges::range_value_t<IdContainer>>;
};

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

    const T& operator[](std::size_t index) const;

    [[nodiscard]] bool empty() const;
    [[nodiscard]] auto size() const;

    void set(const Container& items);

    [[nodiscard]] const Container& getItems() const;

    void add(const T& item)
    requires std::same_as<Container, std::vector<T>>;
    void add(const Container& items)
    requires std::same_as<Container, std::vector<T>>;

    template <typename IdType>
    requires HasId<T, IdType>
    bool remove(IdType id);
    template <typename IdContainer>
    requires HasIdRange<IdContainer, T>
    void remove(const IdContainer& ids);

    template <typename IdType>
    requires HasId<T, IdType>
    bool contains(IdType id) const;

   protected:
    [[nodiscard]] Container& getItems();

    template <typename IdType, typename Hash = std::hash<IdType>>
    [[nodiscard]]
    std::unordered_set<IdType, Hash> _getIds() const;
};

#ifndef __UTILS__INCLUDE__UTILS__ITERABLE_TPP__
#include "iterable.tpp"
#endif

#endif   // __UTILS__INCLUDE__UTILS__ITERABLE_HPP__