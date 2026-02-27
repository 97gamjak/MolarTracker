#ifndef __ORM__CRUD__CRUD_DETAIL_HPP__
#define __ORM__CRUD__CRUD_DETAIL_HPP__

#include <cstddef>
#include <string>
#include <vector>

#include "db/database.hpp"
#include "db/statement.hpp"
#include "orm/constraints.hpp"

namespace orm
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

    // NOLINTBEGIN
#define ORM_CONSTRAINT_MODE(X) \
    X(All)                     \
    X(Not)                     \
    X(Only)

    MSTD_ENUM(ORMConstraintMode, uint8_t, ORM_CONSTRAINT_MODE);
    // NOLINTEND

    template <typename FieldRange>
    std::vector<std::string> getColumnNames(const FieldRange& fields);

    template <typename FieldRange>
    std::vector<std::string> getColumnNames(
        const FieldRange& fields,
        ORMConstraint     constraint,
        ORMConstraintMode mode
    );

    template <typename FieldRange>
    std::vector<std::string> getColumnNames(
        const FieldRange&  fields,
        ORMConstraint      constraint,
        ORMConstraintMode  mode,
        const std::string& suffix
    );

    template <typename FieldRange>
    void bindFieldsToStatement(
        db::Statement&    statement,
        const FieldRange& fields,
        ORMConstraint     constraint,
        ORMConstraintMode mode
    );

    template <typename Model>
    Model loadModelFromStatement(db::Statement const& statement);

    template <typename Model>
    std::size_t getNumberOfPkFields();

}   // namespace orm

#ifndef __ORM__CRUD__CRUD_DETAIL_TPP__
#include "crud_detail.tpp"
#endif   // __ORM__CRUD__CRUD_DETAIL_TPP__

#endif   // __ORM__CRUD__CRUD_DETAIL_HPP__