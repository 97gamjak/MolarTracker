#ifndef __ORM__INCLUDE__ORM__CRUD_TPP__
#define __ORM__INCLUDE__ORM__CRUD_TPP__

#include <cstdint>
#include <expected>
#include <mstd/error.hpp>
#include <mstd/string.hpp>
#include <optional>
#include <vector>

#include "crud/crud_error.hpp"
#include "db/database.hpp"
#include "db/statement.hpp"
#include "db/transaction.hpp"
#include "orm/crud.hpp"
#include "orm/crud/crud_detail.hpp"
#include "orm/fields.hpp"
#include "orm/type_traits.hpp"
#include "where_clause.hpp"

namespace orm
{
    /******************
     * CREATE METHODS *
     ******************/

    /**
     * @brief Create a table for the specified model in the database
     *
     * @tparam Model
     * @param database
     */
    template <db_model Model>
    void Crud::createTable(db::Database& database)
    {
        createTable<Model>(database, Model::tableName);
    }

    /**
     * @brief Create a table for the specified model in the database
     *
     * @tparam Model
     * @param database
     * @param tableName
     */
    template <db_model Model>
    void Crud::createTable(db::Database& database, std::string_view tableName)
    {
        std::string sqlText  = "CREATE TABLE IF NOT EXISTS ";
        sqlText             += tableName;
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

        _sqlExecutions.push_back(sqlText);
    }

    /******************
     * INSERT METHODS *
     ******************/

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
    std::expected<std::int64_t, CrudError> Crud::insert(
        db::Database& database,
        const Model&  row
    )
    {
        db::Transaction transaction{database};
        auto            result = insert(database, transaction, row);

        if (result.has_value())
            transaction.commit();

        return result;
    }

    /**
     * @brief Insert a row into the database
     *
     * @tparam Model
     * @param database
     * @param - transaction An active transaction to use for the insert
     * operation, this is used to ensure that the caller has control over
     * the transaction scope and can manage it as needed (e.g., commit,
     * rollback, etc.), the method will not manage the transaction itself,
     * it will only use it to ensure that the insert operation is performed
     * within the context of the provided transaction.
     * @param row
     * @return std::expected<std::int64_t, CrudError> The ID of the inserted
     * row or an error
     */
    template <db_model Model>
    std::expected<std::int64_t, CrudError> Crud::insert(
        db::Database& database,
        const db::Transaction& /*transaction*/,
        const Model& row
    )
    {
        auto sqlText = "INSERT INTO " + Model::tableName + " (";

        std::vector<std::string> columnNames;
        Model::forEachColumn(
            [&](const auto& field)
            {
                if (field.isAutoIncrementPk)
                    return;

                columnNames.push_back(field.getColumnName());
            }
        );

        sqlText += mstd::join(columnNames, ", ");
        sqlText += ") VALUES (";

        const std::vector<std::string> placeholders(columnNames.size(), "?");
        sqlText += mstd::join(placeholders, ", ");

        sqlText += ");";

        LOG_DEBUG(
            std::format(
                "Inserting into table '{}' with SQL: {}",
                Model::tableName,
                sqlText
            )
        );

        auto statement = database.prepare(sqlText);

        _sqlExecutions.push_back(sqlText);

        std::size_t counter = 0;

        row.forEachField(
            [&](const auto& field)
            {
                if (field.isAutoIncrementPk)
                    return;

                field.bind(statement, bindIndex(counter));
                ++counter;
            }
        );

        statement.executeToCompletion();

        const auto lastInsertId = database.getLastInsertRowid();
        if (lastInsertId.has_value())
            return lastInsertId.value();

        const auto error = CrudError{
            CrudErrorType::InsertFailed,
            "Failed to retrieve last insert ID after insert operation"
        };

        return std::unexpected(error);
    }

    /**
     * @brief Insert multiple rows into the database
     *
     * @tparam Models
     * @param database
     * @param rows
     *
     * @return std::expected<std::vector<std::int64_t>, CrudError>
     */
    template <typename... Models>
    requires(db_model<Models> && ...)
    std::expected<std::vector<std::int64_t>, CrudError> Crud::batchInsert(
        db::Database& database,
        const Models&... rows
    )
    {
        std::vector<std::int64_t> insertedIds;
        insertedIds.reserve(sizeof...(Models) + 1);

        db::Transaction transaction{database};

        std::expected<void, CrudError> batchResult;

        (
            [&](const auto& row)
            {
                if (!batchResult.has_value())
                    return;

                auto result = insert(database, transaction, row);
                if (!result.has_value())
                {
                    batchResult = std::unexpected(result.error());
                    return;
                }

                insertedIds.push_back(result.value());
            }(rows),
            ...
        );

        if (!batchResult.has_value())
            return std::unexpected(batchResult.error());

        transaction.commit();
        return insertedIds;
    }

    /******************
     * UPDATE METHODS *
     ******************/

    /**
     * @brief Update a row in the database
     *
     * @tparam Model
     * @param database
     * @param row
     * @return std::expected<void, CrudError> An empty expected on success,
     * or an error on failure
     */
    template <db_model Model>
    std::expected<void, CrudError> Crud::update(
        db::Database& database,
        const Model&  row
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

        LOG_DEBUG(
            std::format(
                "Updating table '{}' with SQL: {}",
                Model::tableName,
                sqlText
            )
        );
        db::Statement statement = database.prepare(sqlText);

        _sqlExecutions.push_back(sqlText);

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

        const auto changes = database.getNumberOfLastChanges();

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

    /***************
     * GET METHODS *
     ***************/

    /**
     * @brief Get the By Pk object
     *
     * @tparam Model
     * @param database
     * @param model A model instance with the primary key fields set to the
     * desired values
     * @return std::optional<Model>
     */
    template <db_model Model>
    std::optional<Model> Crud::getByPk(
        db::Database& database,
        const Model&  model
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

    /*******************
     * GET ALL METHODS *
     *******************/

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
    std::vector<Model> Crud::getAll(
        db::Database&       database,
        const Joins&        joins,
        const WhereClauses& whereClauses
    )
    {
        std::string sqlText;
        sqlText += "SELECT ";

        sqlText += getColumnNames<Model>(", ") + " ";
        sqlText += joins.getDBOperations(Model::tableName) + " ";
        sqlText += whereClauses.getDBOperations() + ";";

        LOG_DEBUG(
            std::format(
                "Getting from table '{}' with SQL: {}",
                Model::tableName,
                sqlText
            )
        );

        db::Statement statement = database.prepare(sqlText);

        _sqlExecutions.push_back(sqlText);

        whereClauses.bind(statement);

        std::vector<Model> results;

        while (statement.step() == db::StepResult::RowAvailable)
        {
            results.push_back(loadModelFromStatement<Model>(statement));
        }

        return results;
    }

    /**
     * @brief Get all rows matching the specified where clauses
     *
     * @tparam Model
     * @param database
     * @param whereClauses
     * @return std::vector<Model>
     */
    template <db_model Model>
    std::vector<Model> Crud::getAll(
        db::Database&       database,
        const WhereClauses& whereClauses
    )
    {
        return getAll<Model>(database, Joins{}, whereClauses);
    }

    /**
     * @brief Get all rows matching the specified where clauses
     *
     * @tparam Model
     * @param database
     * @return std::vector<Model>
     */
    template <db_model Model>
    std::vector<Model> Crud::getAll(db::Database& database)
    {
        return getAll<Model>(database, WhereClauses{});
    }

    /**
     * @brief Get a unique row matching the specified where clause
     *
     * @tparam Model
     * @param database
     * @param whereClauses
     * @return std::expected<Model, CrudError> The unique model matching the
     * where clause, or an error if no results or multiple results are found
     */
    template <db_model Model>
    std::expected<Model, CrudError> Crud::getUnique(
        db::Database&       database,
        const WhereClauses& whereClauses
    )
    {
        const auto results = getAll<Model>(database, whereClauses);

        if (results.empty())
        {
            const auto msg =
                "No results found for getUnique with where clause: " +
                whereClauses.getDBOperations();

            return std::unexpected(CrudError{CrudErrorType::NotFound, msg});
        }

        if (results.size() > 1)
        {
            const auto msg = "Expected unique result for getUnique but got " +
                             std::to_string(results.size());

            return std::unexpected(
                CrudError{CrudErrorType::MultipleResults, msg}
            );
        }

        return results.front();
    }

    /******************
     * DELETE METHODS *
     ******************/

    /**
     * @brief Delete a row by primary keys
     *
     * @tparam Model
     * @param database
     * @param model
     */
    template <db_model Model>
    void Crud::deleteByPk(db::Database& database, const Model& model)
    {
        const auto numberOfPkFields = getNumberOfPkFields<Model>();

        if (numberOfPkFields != 1)
        {
            throw CrudException(
                "orm::deleteByPk requires a model with exactly one primary "
                "key field"
            );
        }

        std::string sqlText;
        sqlText += "DELETE FROM ";
        sqlText += Model::tableName;
        sqlText += " WHERE ";

        const auto whereClauses = getPkWhereClauses<Model>(model);

        if (whereClauses.empty())
        {
            throw CrudException("orm::deleteByPk requires a primary key field");
        }

        if (whereClauses.size() != numberOfPkFields)
        {
            throw CrudException(
                "orm::deleteByPk requires all primary key fields to be set"
            );
        }

        sqlText += whereClauses.getDBOperations();
        sqlText += ";";

        LOG_DEBUG(
            std::format(
                "Deleting from table '{}' with SQL: {}",
                Model::tableName,
                sqlText
            )
        );

        db::Statement statement = database.prepare(sqlText);

        _sqlExecutions.push_back(sqlText);

        whereClauses.bind(statement);

        statement.executeToCompletion();
    }

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__CRUD_TPP__