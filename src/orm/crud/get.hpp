#ifndef __ORM__CRUD__GET_HPP__
#define __ORM__CRUD__GET_HPP__

#include <mstd/string.hpp>
#include <string>
#include <vector>

#include "crud_error.hpp"
#include "db/database.hpp"
#include "db/statement.hpp"
#include "db/transaction.hpp"
#include "logging/log_macros.hpp"
#include "orm/join.hpp"
#include "orm/type_traits.hpp"
#include "orm/where_clause.hpp"

REGISTER_LOG_CATEGORY("Orm.Crud.Get");

namespace orm::details
{
    /**
     * @brief Get a single row by primary key value
     *
     * @tparam Model
     * @param database
     * @param model
     * @return std::optional<Model> The loaded model if found, or std::nullopt
     * if not found
     */
    template <db_model Model>
    [[nodiscard]] std::optional<Model> getByPk(
        const std::shared_ptr<db::Database>& database,
        const Model&                         model
    )
    {
        const auto numberOfPkFields = getNumberOfPkFields<Model>();

        const auto whereClauses = getPkWhereClauses<Model>(model);

        if (whereClauses.empty())
        {
            throw CrudException(
                "orm::getByPk requires at least one primary key field"
            );
        }

        if (whereClauses.size() != numberOfPkFields)
        {
            throw CrudException(
                "orm::getByPk requires all primary key fields to be set"
            );
        }

        const auto returnedModels =
            getAll<Model>(database, Joins{}, whereClauses);

        if (returnedModels.size() > 1)
        {
            throw CrudException(
                "orm::getByPk expected to find at most one row matching the "
                "primary key value, but found multiple rows"
            );
        }

        if (returnedModels.empty())
            return std::nullopt;

        return returnedModels.front();
    }

    /**
     * @brief Get all rows matching the specified where clauses
     *
     * @tparam Model
     * @param database
     * @param joins
     * @param whereClauses
     * @return std::vector<Model>
     */
    template <db_model Model>
    [[nodiscard]] std::vector<Model> getAll(
        const std::shared_ptr<db::Database>& database,
        const Joins&                         joins,
        const WhereClauses&                  whereClauses
    )
    {
        std::string sqlText;
        sqlText += "SELECT ";

        sqlText += getColumnNames<Model>(", ") + " ";
        sqlText += joins.getDBOperations(Model::tableName) + " ";
        sqlText += whereClauses.getDBOperations() + ";";

        if (database == nullptr)
            throw CrudException("Database pointer is null");

        LOG_DEBUG(
            std::format(
                "Getting from table '{}' with SQL: {}",
                Model::tableName,
                sqlText
            )
        );

        db::Statement statement = database->prepare(sqlText);

        whereClauses.bind(statement);

        std::vector<Model> results;

        while (statement.step() == db::StepResult::RowAvailable)
        {
            results.push_back(loadModelFromStatement<Model>(statement));
        }

        return results;
    }

}   // namespace orm::details

#endif   // __ORM__CRUD__GET_HPP__