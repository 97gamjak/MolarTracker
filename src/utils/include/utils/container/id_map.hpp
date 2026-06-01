#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_HPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__ID_MAP_HPP__

#include "map.hpp"

template <typename Value>
requires HasId<Value>
class IdMap : public Map<IdOf<Value>, Value, typename IdOf<Value>::Hash>
{
   private:
    using Base = Map<IdOf<Value>, Value, typename IdOf<Value>::Hash>;

    using Base::_items;

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