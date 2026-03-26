#ifndef __ORM__INCLUDE__ORM__CRUD__CRUD_DETAIL_HPP__
#define __ORM__INCLUDE__ORM__CRUD__CRUD_DETAIL_HPP__

#include <cstddef>
#include <cstdint>
#include <mstd/enum.hpp>
#include <string>

namespace orm
{
    namespace details
    {
        template <typename Model, typename Group, std::size_t... I>
        void appendUniqueGroupImpl(
            std::string& sql,
            std::index_sequence<I...> /*dummy*/
        );

        template <typename Model, typename Group>
        void appendUniqueGroup(std::string& sql);

        template <typename Model, typename Groups, std::size_t... I>
        void appendAllUniqueGroups(
            std::string& sql,
            Groups /*groups*/,
            std::index_sequence<I...> /*dummy*/
        );
    }   // namespace details

    template <typename Model>
    void appendAllUniqueGroups(std::string& sql);

    // NOLINTBEGIN
#define ORM_CONSTRAINT_MODE(X) \
    X(All)                     \
    X(Not)                     \
    X(Only)

    MSTD_ENUM(ORMConstraintMode, uint8_t, ORM_CONSTRAINT_MODE);
    // NOLINTEND

}   // namespace orm

#ifndef __ORM__INCLUDE__ORM__CRUD__CRUD_DETAIL_TPP__
#include "crud_detail.tpp"
#endif   // __ORM__INCLUDE__ORM__CRUD__CRUD_DETAIL_TPP__

#endif   // __ORM__INCLUDE__ORM__CRUD__CRUD_DETAIL_HPP__