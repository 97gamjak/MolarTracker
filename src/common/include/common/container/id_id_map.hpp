#ifndef __COMMON__INCLUDE__COMMON__CONTAINER__ID_ID_MAP_HPP__
#define __COMMON__INCLUDE__COMMON__CONTAINER__ID_ID_MAP_HPP__

#include <string>

#include "map.hpp"

/**
 * @brief A mapping of IDs to other IDs, this is used in the store to keep track
 * of ID remapping during commit operations, allowing the store to maintain a
 * mapping of original IDs to their new IDs after changes have been committed,
 * which can be useful for handling cases where IDs are generated or changed
 * during the commit process, and provides a way to look up the new ID for a
 * given original ID after a commit has been performed.
 *
 * @tparam IdType
 * @tparam SecondIdType defaults to IdType, this allows for flexibility in the
 * types of IDs being mapped, while still providing a convenient default for
 * cases where the IDs being mapped are of the same type, and allows for the
 * possibility of mapping between different types of IDs if needed.
 */
template <typename IdType, typename SecondIdType = IdType>
class IdIdMap : public Map<IdType, SecondIdType, typename IdType::Hash>
{
   private:
    /// The base class type
    using Base = Map<IdType, SecondIdType, typename IdType::Hash>;

   public:
    using Base::Base;

    [[nodiscard]] bool combine(const IdIdMap& other);

    [[nodiscard]] std::string toString() const;
};

#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__ID_ID_MAP_TPP__
#include "id_id_map.tpp"
#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__ID_ID_MAP_TPP__

#endif   // __COMMON__INCLUDE__COMMON__CONTAINER__ID_ID_MAP_HPP__