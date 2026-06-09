#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_HPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_HPP__

#include <vector>

#include "map.hpp"

template <typename T>
concept HasId = requires(T item) {
    { item.getId() };
};

template <typename T>
using IdOf = decltype(std::declval<T>().getId());

/**
 * @brief A map that associates IDs with values.
 *
 * @tparam Value The type of the values in the map.
 */
template <typename Value>
requires HasId<Value>
class IdMap : public Map<IdOf<Value>, Value, typename IdOf<Value>::Hash>
{
   private:
    /// The base class type
    using Base = Map<IdOf<Value>, Value, typename IdOf<Value>::Hash>;

    /// The key type
    using Key = IdOf<Value>;

   public:
    using Base::Base;

    template <std::ranges::range R>
    void setUnchecked(R&& values);

    [[nodiscard]] bool add(const Value& value);
    void               addUnchecked(const Value& value);
    template <std::ranges::range R>
    void addUnchecked(R&& values);

    [[nodiscard]] std::unordered_set<Key, typename Key::Hash> getIds() const;
};

#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_TPP__
#include "id_map.tpp"
#endif

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_HPP__