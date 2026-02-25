#include "db/database.hpp"
#include "orm/fields.hpp"
#include "orm/type_traits.hpp"

namespace orm
{
    /**
     * @brief Create a table for the specified model if it doesn't exist
     *
     * @tparam Model
     * @param database
     */
    template <typename Model, typename Group, std::size_t... I>
    void append_unique_group_sql_impl(
        std::string& sql,
        std::index_sequence<I...>
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
    void append_unique_group_sql(std::string& sql)
    {
        constexpr auto size = std::tuple_size_v<decltype(Group::members)>;

        append_unique_group_sql_impl<Model, Group>(sql, index_seq<size>{});
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
    void append_all_unique_groups_sql(
        std::string& sql,
        Groups,
        std::index_sequence<I...>
    )
    {
        (append_unique_group_sql<Model, std::tuple_element_t<I, Groups>>(sql),
         ...);
    }

    /**
     * @brief Create a table for the specified model in the database
     *
     * @tparam Model
     * @param database
     */
    template <db_model Model>
    void create_table(db::Database& database)
    {
        std::string sqlText  = "CREATE TABLE IF NOT EXISTS ";
        sqlText             += Model::table_name;
        sqlText             += " (";

        auto firstCol = true;

        for (const auto& field : orm::fields<Model>())
        {
            if (!firstCol)
                sqlText += ", ";

            firstCol  = false;
            sqlText  += field.ddl();
        }

        if constexpr (has_unique_groups<Model>)
        {
            auto const            groups = Model::getUniqueGroups();
            constexpr std::size_t size   = std::tuple_size_v<decltype(groups)>;
            append_all_unique_groups_sql<Model>(
                sqlText,
                groups,
                index_seq<size>{}
            );
        }

        sqlText += ");";

        database.execute(sqlText);
    }
}   // namespace orm