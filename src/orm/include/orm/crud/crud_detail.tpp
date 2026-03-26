#ifndef __ORM__INCLUDE__ORM__CRUD__CRUD_DETAIL_TPP__
#define __ORM__INCLUDE__ORM__CRUD__CRUD_DETAIL_TPP__

#include "crud_detail.hpp"
#include "orm/type_traits.hpp"

namespace orm
{
    namespace details
    {
        /**
         * @brief Create a table for the specified model if it doesn't exist
         *
         * @tparam Model
         * @param database
         */
        template <db_model Model, typename Group, std::size_t... I>
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

                sql += (Model{}.*member).getColumnName().toString();
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
        template <db_model Model, typename Group>
        void appendUniqueGroup(std::string& sql)
        {
            constexpr auto size = std::tuple_size_v<decltype(Group::members)>;

            appendUniqueGroupImpl<Model, Group>(
                sql,
                std::index_sequence<size>{}
            );
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
        template <db_model Model, typename Groups, std::size_t... I>
        void appendAllUniqueGroups(
            std::string& sql,
            Groups /*groups*/,
            std::index_sequence<I...> /*dummy*/
        )
        {
            (appendUniqueGroup<Model, std::tuple_element_t<I, Groups>>(sql),
             ...);
        }

    }   // namespace details

    /**
     * @brief Insert a row into the database
     *
     * @tparam Model
     * @param database
     * @param row
     * @return std::expected<std::int64_t, CrudError> The ID of the inserted
     * row or an error
     */
    template <db_model Model>
    void appendAllUniqueGroups(std::string& sql)
    {
        const auto     groups = Model::getUniqueGroups();
        constexpr auto size   = std::tuple_size_v<decltype(groups)>;
        appendAllUniqueGroups<Model>(
            sql,
            groups,
            std::make_index_sequence<size>{}
        );
    }

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__CRUD__CRUD_DETAIL_TPP__