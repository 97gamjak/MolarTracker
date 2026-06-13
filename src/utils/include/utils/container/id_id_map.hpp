#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ID_ID_MAP_HPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__ID_ID_MAP_HPP__

#include "map.hpp"

template <typename IdType, typename SecondIdType = IdType>
class IdIdMap : public Map<IdType, SecondIdType, typename IdType::Hash>
{
   private:
    /// The base class type
    using Base = Map<IdType, SecondIdType, typename IdType::Hash>;

   public:
    using Base::Base;

    [[nodiscard]] bool combine(const IdIdMap& other);
};

#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ID_ID_MAP_TPP__
#include "id_id_map.tpp"
#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__ID_ID_MAP_TPP__

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__ID_ID_MAP_HPP__