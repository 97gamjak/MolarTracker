#ifndef __ORM__CRUD__CRUD_DETAIL_HPP__
#define __ORM__CRUD__CRUD_DETAIL_HPP__

#include <cstddef>
#include <string>

#include "orm/fields.hpp"

namespace orm
{
    /**
     * @brief Create a table for the specified model if it doesn't exist
     *
     * @tparam Model
     * @param database
     */
    template <typename Model, typename Group, std::size_t... I>
    void appendUniqueGroupImpl(
        std::string& sql,
        std::index_sequence<I...> /*dummy*/
    )
    {
        sql += ", UNIQUE (";

        bool first = true;

        auto append_column = [&](auto member)
        {
            if (!first)
                sql += ", ";
            first = false;

            sql += std::string((Model{}.*member).getColumnName().view());
        };

        auto const& members = Group::members;

        (std::apply(
            [&](auto... member) { (append_column(member), ...); },
            members
        ));

        sql += ")";
    }

    /**
     * @brief Append SQL for a unique group to the provided SQL string
     *
     * @tparam Model
     * @tparam Group
     * @param sql
     */
    template <typename Model, typename Group>
    void appendUniqueGroup(std::string& sql)
    {
        constexpr auto size = std::tuple_size_v<decltype(Group::members)>;

        appendUniqueGroupImpl<Model, Group>(sql, index_seq<size>{});
    }

    /**
     * @brief Append SQL for all unique groups of a model
     *
     * @tparam Model
     * @tparam Groups
     * @tparam I
     * @param sql
     * @param groups
     * @param index_sequence
     */
    template <typename Model, typename Groups, std::size_t... I>
    void appendAllUniqueGroups(
        std::string& sql,
        Groups /*groups*/,
        std::index_sequence<I...> /*dummy*/
    )
    {
        (appendUniqueGroup<Model, std::tuple_element_t<I, Groups>>(sql), ...);
    }
}   // namespace orm

#endif   // __ORM__CRUD__CRUD_DETAIL_HPP__