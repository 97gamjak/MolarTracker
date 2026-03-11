#ifndef __ORM__CRUD_HPP__
#define __ORM__CRUD_HPP__

#include <cstdint>
#include <expected>
#include <mstd/error.hpp>
#include <optional>
#include <vector>

#include "constraints.hpp"
#include "crud/create.hpp"
#include "crud/crud_error.hpp"
#include "crud/delete.hpp"
#include "crud/get.hpp"
#include "crud/insert.hpp"
#include "crud/update.hpp"
#include "db/database.hpp"
#include "db/transaction.hpp"
#include "type_traits.hpp"
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
    void createTable(const std::shared_ptr<db::Database>& database)
    {
        details::createTable<Model>(database);
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
     * @return std::expected<std::int64_t, CrudError> The ID of the inserted row
     * or an error
     */
    template <db_model Model>
    requires is_freely_insertable_v<Model>
    [[nodiscard]] std::expected<std::int64_t, CrudError> insert(
        const std::shared_ptr<db::Database>& database,
        const Model&                         row
    )
    {
        db::Transaction transaction{database};
        auto            result = details::_insert(database, row);

        if (result.has_value())
            transaction.commit();

        return result;
    }

    /**
     * @brief Insert a row into the database
     *
     * @tparam Model
     * @param database
     * @param transaction An active transaction to use for the insert operation,
     * this is used to ensure that the caller has control over the transaction
     * scope and can manage it as needed (e.g., commit, rollback, etc.), the
     * method will not manage the transaction itself, it will only use it to
     * ensure that the insert operation is performed within the context of the
     * provided transaction.
     * @param row
     * @return std::expected<std::int64_t, CrudError> The ID of the inserted row
     * or an error
     */
    template <db_model Model>
    [[nodiscard]] std::expected<std::int64_t, CrudError> insert(
        const std::shared_ptr<db::Database>& database,
        const db::Transaction& /*transaction*/,
        const Model& row
    )
    {
        return details::_insert(database, row);
    }

    /**
     * @brief Insert multiple rows into the database
     *
     * @tparam Model1
     * @tparam Models
     */
    template <typename... Models>
    requires(db_model<Models> && ...)
    [[nodiscard]] std::expected<std::vector<std::int64_t>, CrudError> batchInsert(
        const std::shared_ptr<db::Database>& database,
        const Models&... rows
    )
    {
        return details::batchInsert(database, rows...);
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
     * @return std::expected<void, CrudError> An empty expected on success, or
     * an error on failure
     */
    template <db_model Model>
    [[nodiscard]] std::expected<void, CrudError> update(
        const std::shared_ptr<db::Database>& database,
        const Model&                         row
    )
    {
        return details::update(database, row);
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
    [[nodiscard]] std::optional<Model> getByPk(
        const std::shared_ptr<db::Database>& database,
        const Model&                         model
    )
    {
        return details::getByPk<Model>(database, model);
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
    [[nodiscard]] std::vector<Model> getAll(
        const std::shared_ptr<db::Database>& database,
        const Joins&                         joins,
        const WhereClauses&                  whereClauses
    )
    {
        return details::getAll<Model>(database, joins, whereClauses);
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
    [[nodiscard]] std::vector<Model> getAll(
        const std::shared_ptr<db::Database>& database,
        const WhereClauses&                  whereClauses
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
    [[nodiscard]] std::vector<Model> getAll(
        const std::shared_ptr<db::Database>& database
    )
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
    [[nodiscard]] std::expected<Model, CrudError> getUnique(
        const std::shared_ptr<db::Database>& database,
        const WhereClauses&                  whereClauses
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
    void deleteByPk(
        const std::shared_ptr<db::Database>& database,
        const Model&                         model
    )
    {
        details::deleteByPk<Model>(database, model);
    }

}   // namespace orm

#endif   // __ORM__CRUD_HPP__