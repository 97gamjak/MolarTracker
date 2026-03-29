#ifndef __ORM__INCLUDE__ORM__CRUD__CREATE_HPP__
#define __ORM__INCLUDE__ORM__CRUD__CREATE_HPP__

#include <mstd/string.hpp>

#include "db/database.hpp"
#include "logging/log_macros.hpp"
#include "orm/fields.hpp"
#include "orm/type_traits.hpp"

REGISTER_LOG_CATEGORY("Orm.Crud.Create");

namespace orm::details
{
    /**
     * @brief Create a table for the specified model in the database
     *
     * @tparam Model
     * @param database
     */
    template <db_model Model>
    void createTable(db::Database& database)
    {
        std::string sqlText  = "CREATE TABLE IF NOT EXISTS ";
        sqlText             += Model::tableName;
        sqlText             += " (";

        const auto ddl  = getDDl<Model>();
        sqlText        += mstd::join(ddl, ", ");

        const auto fkConstraints = getFKConstraints<Model>();
        if (!fkConstraints.empty())
            sqlText += ", " + mstd::join(fkConstraints, ", ");

        if constexpr (has_unique_groups<Model>)
            appendAllUniqueGroups<Model>(sqlText);

        sqlText += ");";

        LOG_DEBUG(
            std::format(
                "Creating table '{}' with SQL: {}",
                Model::tableName,
                sqlText
            )
        );

        database.execute(sqlText);
    }
}   // namespace orm::details

#endif   // __ORM__INCLUDE__ORM__CRUD__CREATE_HPP__