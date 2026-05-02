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
#include "filter/expr_node.hpp"
#include "orm/crud.hpp"
#include "orm/crud/crud_detail.hpp"
#include "orm/fields.hpp"
#include "orm/index.hpp"
#include "orm/query_options.hpp"
#include "orm/type_traits.hpp"
#include "orm/where_expr.hpp"
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
        std::size_t nInsertableFields = 0;
        Model::forEachColumn(
            [&](const auto& field)
            {
                if (field.isAutoIncrementPk)
                    return;

                ++nInsertableFields;
            }
        );

        auto sqlText = "INSERT INTO " + Model::tableName + " ";

        if (nInsertableFields > 0)
        {
            sqlText += "(";

            std::vector<std::string> columnNames;
            Model::forEachColumn(
                [&](const auto& field)
                {
                    if (field.isAutoIncrementPk)
                        return;

                    columnNames.push_back(std::string(field.name));
                }
            );

            sqlText += mstd::join(columnNames, ", ");
            sqlText += ") VALUES (";

            const std::vector<std::string> placeholders(
                columnNames.size(),
                "?"
            );
            sqlText += mstd::join(placeholders, ", ");
            sqlText += ")";
        }
        else
        {
            sqlText += " DEFAULT VALUES";
        }

        sqlText += ";";

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

                columnNames.push_back(field.name + "=?");
            }
        );

        sqlText += mstd::join(columnNames, ", ");

        const auto where = getPkWhere(row);

        if (filter::isEmpty(where))
        {
            return std::unexpected(CrudError(
                CrudErrorType::NoPrimaryKey,
                "orm::update requires a model with at least one primary key "
                "field"
            ));
        }

        sqlText += getDBOperations(where);
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

        bind(where, statement);

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

    /**
     * @brief Update a single field in the database
     *
     * @tparam Field
     * @param database
     * @param field
     * @return std::expected<void, CrudError> An empty expected on success,
     * or an error on failure
     */
    template <typename Field>
    std::expected<void, CrudError> Crud::updateField(
        db::Database& database,
        const Field&  field
    )
    {
        std::string sqlText;
        sqlText += "UPDATE ";
        sqlText += Field::tableName;
        sqlText += " SET ";

        sqlText += field.name + "=?";
        sqlText += ";";

        LOG_DEBUG(
            std::format(
                "Updating table '{}' with SQL: {}",
                Field::tableName,
                sqlText
            )
        );
        db::Statement statement = database.prepare(sqlText);

        _sqlExecutions.push_back(sqlText);

        field.bind(statement, bindIndex(0));

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

    /*******************
     * GET ALL METHODS *
     *******************/

    /**
     * @brief Get all rows matching the specified query options
     *
     * @tparam Model
     * @param database
     * @param joins
     * @param query
     * @return std::vector<Model>
     */
    template <db_model Model>
    std::vector<Model> Crud::get(
        db::Database& database,
        const Joins&  joins,
        const Query&  query
    )
    {
        std::string sqlText;
        sqlText += getSelection<Model>() + " ";
        sqlText += joins.toSQL() + " ";
        sqlText += query.getDBOperations() + ";";

        LOG_DEBUG(
            std::format(
                "Getting from table '{}' with SQL: {}",
                Model::tableName,
                sqlText
            )
        );

        db::Statement statement = database.prepare(sqlText);

        _sqlExecutions.push_back(sqlText);

        query.bind(statement);

        std::vector<Model> results;

        while (statement.step() == db::StepResult::RowAvailable)
            results.push_back(loadModelFromStatement<Model>(statement));

        return results;
    }

    /**
     * @brief Get all rows matching the specified query options
     *
     * @tparam Model
     * @param database
     * @param query
     * @return std::vector<Model>
     */
    template <db_model Model>
    std::vector<Model> Crud::get(db::Database& database, const Query& query)
    {
        return get<Model>(database, Joins{}, query);
    }

    /**
     * @brief Get all rows
     *
     * @tparam Model
     * @param database
     * @return std::vector<Model>
     */
    template <db_model Model>
    std::vector<Model> Crud::get(db::Database& database)
    {
        return get<Model>(database, Joins{}, Query{});
    }

    /**
     * @brief Get all rows matching the specified query options
     *
     * @tparam Model
     * @param database
     * @param joins
     * @param query
     * @return std::vector<Model>
     */
    template <db_model... Models>
    std::vector<std::tuple<Models...>> Crud::getJoined(
        db::Database&     database,
        const orm::Joins& joins,
        const Query&      query
    )
    {
        std::string sql;
        sql += getSelection<Models...>();
        sql += joins.toSQL() + " ";
        sql += query.getDBOperations() + ";";

        LOG_DEBUG(std::format("Getting joined with SQL: {}", sql));

        db::Statement statement = database.prepare(sql);
        _sqlExecutions.push_back(sql);
        query.bind(statement);

        std::vector<std::tuple<Models...>> results;

        while (statement.step() == db::StepResult::RowAvailable)
            results.push_back(loadTupleFromStatement<Models...>(statement));

        return results;
    }

    /**
     * @brief Get a unique row matching the specified where clauses
     *
     * @tparam Model
     * @param database
     * @param query
     * @return std::optional<Model>
     */
    template <db_model Model>
    std::optional<Model> Crud::getUnique(
        db::Database& database,
        const Query&  query
    )
    {
        const auto results = get<Model>(database, query);

        if (results.empty())
            return std::nullopt;

        if (results.size() > 1)
        {
            const auto msg = std::format(
                "Expected to get a unique result for query, but got {} "
                "results. "
                "This indicates a data integrity issue.",
                results.size()
            );

            LOG_ERROR(msg);
            throw CrudException(msg);
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

        const auto where = getPkWhere(model);

        if (filter::isEmpty(where))
        {
            throw CrudException("orm::deleteByPk requires a primary key field");
        }

        if (filter::getSize(where) != numberOfPkFields)
        {
            throw CrudException(
                "orm::deleteByPk requires all primary key fields to be set"
            );
        }

        sqlText += getDBOperations(where);
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

        bind(where, statement);

        statement.executeToCompletion();
    }

    /***************
     * ADD METHODS *
     ***************/

    /**
     * @brief Add a new column to the database
     *
     * @tparam Field
     * @param database
     * @param field
     * @return std::expected<void, CrudError> An empty expected on success,
     * or an error on failure
     */
    template <typename Field>
    std::expected<void, CrudError> Crud::addColumn(
        db::Database& database,
        const Field&  field
    )
    {
        const auto columnExist = _columnExists(
            database,
            std::string(Field::name),
            std::string(Field::tableName)
        );

        if (columnExist)
        {
            return std::unexpected(CrudError(
                CrudErrorType::ColumnAlreadyExists,
                "Column already exists"
            ));
        }

        std::string sql;
        sql += "ALTER TABLE ";
        sql += Field::tableName;
        sql += " ADD COLUMN ";
        sql += Field::ddl();

        if (Field::isNotNull)
        {
            sql += " DEFAULT ";
            sql += field.valueAsString();
        }

        LOG_DEBUG(std::format("Adding column with SQL: {}", sql));

        db::Statement statement = database.prepare(sql);

        _sqlExecutions.push_back(sql);

        statement.executeToCompletion();

        return {};
    }

    /**
     * @brief Drop a column from the database
     *
     * @tparam Model
     * @param database
     * @param columnName
     * @return std::expected<void, CrudError> An empty expected on success,
     * or an error on failure
     */
    template <typename Model>
    std::expected<void, CrudError> Crud::dropColumn(
        db::Database&      database,
        const std::string& columnName
    )
    {
        if (!_columnExists(database, columnName, std::string(Model::tableName)))
        {
            return std::unexpected(CrudError(
                CrudErrorType::ColumnDoesNotExist,
                "Column does not exist"
            ));
        }

        std::string sql;
        sql += "ALTER TABLE ";
        sql += Model::tableName;
        sql += " DROP COLUMN ";
        sql += columnName;

        LOG_DEBUG(std::format("Dropping column with SQL: {}", sql));

        db::Statement statement = database.prepare(sql);

        _sqlExecutions.push_back(sql);

        statement.executeToCompletion();

        return {};
    }

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__CRUD_TPP__