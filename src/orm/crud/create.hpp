#ifndef __ORM__CRUD__CREATE_HPP__
#define __ORM__CRUD__CREATE_HPP__

#include "crud_detail.hpp"
#include "crud_error.hpp"
#include "db/database.hpp"
#include "orm/fields.hpp"
#include "orm/type_traits.hpp"

namespace orm
{
    /**
     * @brief Create a table for the specified model in the database
     *
     * @tparam Model
     * @param database
     */
    template <db_model Model>
    void createTable(const std::shared_ptr<db::Database>& database)
    {
        std::string sqlText  = "CREATE TABLE IF NOT EXISTS ";
        sqlText             += Model::tableName;
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
            appendAllUniqueGroups<Model>(sqlText, groups, index_seq<size>{});
        }

        sqlText += ");";

        if (database == nullptr)
            throw CrudException("Database pointer is null");

        database->execute(sqlText);
    }
}   // namespace orm

#endif   // __ORM__CRUD__CREATE_HPP__