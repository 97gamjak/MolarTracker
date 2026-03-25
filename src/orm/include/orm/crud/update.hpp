#ifndef __ORM__CRUD__UPDATE_HPP__
#define __ORM__CRUD__UPDATE_HPP__

#include <expected>
#include <mstd/string.hpp>
#include <string>

#include "crud_error.hpp"
#include "db/database.hpp"
#include "db/statement.hpp"
#include "logging/log_macros.hpp"
#include "orm/type_traits.hpp"
#include "orm/where_clause.hpp"

REGISTER_LOG_CATEGORY("Orm.Crud.Update");

namespace orm::details
{

    /**
     * @brief Update a row in the database
     *
     * @tparam Model
     * @param database
     * @param row
     * @return std::expected<void, CrudError> An empty expected on success, or
     * an error on failure
     */
    template <db_model Model>
    std::expected<void, CrudError> update(
        const std::shared_ptr<db::Database>& database,
        const Model&                         row
    )
    {
        std::string sqlText;
        sqlText += "UPDATE ";
        sqlText += Model::tableName;
        sqlText += " SET ";

        std::vector<std::string> columnNames;
        Model::forEachColumn(
            [&](const auto& field)
            {
                if (field.isPk)
                    return;

                columnNames.push_back(field.getColumnName() + "=?");
            }
        );

        sqlText += mstd::join(columnNames, ", ");

        const auto whereClauses = getPkWhereClauses(row);

        if (whereClauses.empty())
        {
            return std::unexpected(CrudError(
                CrudErrorType::NoPrimaryKey,
                "orm::update requires a model with at least one primary key "
                "field"
            ));
        }

        sqlText += whereClauses.getDBOperations();
        sqlText += ";";

        if (database == nullptr)
            throw CrudException("Database pointer is null");

        LOG_DEBUG(
            std::format(
                "Updating table '{}' with SQL: {}",
                Model::tableName,
                sqlText
            )
        );
        db::Statement statement = database->prepare(sqlText);

        std::size_t index = 0;
        row.forEachField(
            [&](const auto& field)
            {
                if (field.isPk)
                    return;

                field.bind(statement, bindIndex(index));
                ++index;
            }
        );

        whereClauses.bind(statement);

        statement.executeToCompletion();

        const auto changes = database->getNumberOfLastChanges();

        if (changes == 0)
        {
            return std::unexpected(CrudError(
                CrudErrorType::NoRowsUpdated,
                "orm::update did not update any rows. This may be because the "
                "primary key value(s) did not match any existing row."
            ));
        }

        if (changes > 1)
        {
            return std::unexpected(CrudError(
                CrudErrorType::MultipleRowsUpdated,
                "orm::update updated multiple rows. This should never happen, "
                "as updates are performed by matching primary key values."
            ));
        }

        return {};
    }
}   // namespace orm::details

#endif   // __ORM__CRUD__UPDATE_HPP__