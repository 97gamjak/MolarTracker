#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_HPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_HPP__

#include "config/strong_id.hpp"
#include "map.hpp"

/**
 * @brief A map that associates IDs with values.
 *
 * @tparam Key The type of the keys in the map, which must be a strong ID type.
 * @tparam Value The type of the values in the map.
 */
template <IsId Key, typename Value>
class IdMap : public Map<Key, Value, typename Key::Hash>
{
   private:
    /// The base class type
    using Base = Map<Key, Value, typename Key::Hash>;

   public:
    using Map<Key, Value, typename Key::Hash>::Map;

    [[nodiscard]] std::unordered_set<Key, typename Key::Hash> getIds() const;
};

/**
 * @brief A map that associates IDs with values.
 *
 * @tparam Value The type of the values in the map.
 */
template <typename Value>
requires HasId<Value>
class IdObjectMap : public IdMap<IdOf<Value>, Value>
{
   private:
    /// The base class type
    using Base = IdMap<IdOf<Value>, Value>;

    /// The key type
    using Key = IdOf<Value>;

   public:
    using IdMap<IdOf<Value>, Value>::IdMap;

    // NOLINTBEGIN(google-explicit-constructor, hicpp-explicit-conversions)
    template <std::ranges::range R>
    IdObjectMap(R&& values);
    // NOLINTEND(google-explicit-constructor, hicpp-explicit-conversions)

    template <std::ranges::range R>
    void setUnchecked(R&& values);

    [[nodiscard]] bool add(const Value& value);
    void               addUnchecked(const Value& value);
    template <std::ranges::range R>
    void addUnchecked(R&& values);
};

#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_TPP__
#include "id_map.tpp"
#endif

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_HPP__