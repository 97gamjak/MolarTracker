#ifndef __ORM__INCLUDE__ORM__CRUD_HPP__
#define __ORM__INCLUDE__ORM__CRUD_HPP__

#include <mstd/error.hpp>
#include <optional>
#include <vector>

#include "db/database.hpp"
#include "db/transaction.hpp"
#include "error/crud_error.hpp"
#include "join.hpp"
#include "orm/type_traits.hpp"
#include "query_options.hpp"

namespace orm
{
    /**
     * @brief Class for handling CRUD operations
     *
     */
    class Crud
    {
       private:
        /// A vector to store executed SQL statements
        std::vector<std::string> _sqlExecutions;

       public:
        [[nodiscard]] const std::vector<std::string>& getExecutedSQL() const;

        /******************
         * CREATE METHODS *
         ******************/

        template <db_model Model>
        [[nodiscard]]
        CrudResult<void> createTable(db::Database& database);

        template <db_model Model>
        [[nodiscard]]
        CrudResult<void> createTable(
            db::Database&    database,
            std::string_view tableName
        );

        /******************
         * INSERT METHODS *
         ******************/

        template <db_model Model>
        [[nodiscard]]
        CrudResult<std::int64_t> insert(
            db::Database& database,
            const Model&  row
        );

        template <db_model Model>
        [[nodiscard]]
        CrudResult<std::int64_t> insert(
            db::Database& database,
            const db::Transaction& /*transaction*/,
            const Model& row
        );

        template <typename... Models>
        requires(db_model<Models> && ...)
        [[nodiscard]]
        CrudResult<std::vector<std::int64_t>> batchInsert(
            db::Database& database,
            const Models&... rows
        );

        /******************
         * UPDATE METHODS *
         ******************/

        template <db_model Model>
        [[nodiscard]]
        CrudResult<void> update(db::Database& database, const Model& row);

        template <typename Field>
        [[nodiscard]]
        CrudResult<void> updateField(
            db::Database& database,
            const Field&  field
        );

        /*******************
         * GET ALL METHODS *
         *******************/

        template <db_model Model>
        [[nodiscard]] std::vector<Model> get(
            db::Database& database,
            const Joins&  joins,
            const Query&  query
        );

        template <db_model Model>
        [[nodiscard]] std::vector<Model> get(
            db::Database& database,
            const Query&  query
        );

        template <db_model Model>
        [[nodiscard]] std::vector<Model> get(db::Database& database);

        template <typename... Models>
        std::vector<std::tuple<Models...>> getJoined(
            db::Database&     database,
            const orm::Joins& joins,
            const Query&      query
        );

        template <db_model Model>
        [[nodiscard]] std::optional<Model> getUnique(
            db::Database& database,
            const Query&  query
        );

        /******************
         * DELETE METHODS *
         ******************/

        template <db_model Model>
        void deleteByPk(db::Database& database, const Model& model);

        /******************
         * COLUMN METHODS *
         ******************/

        template <typename Field>
        [[nodiscard]]
        CrudResult<void> addColumn(db::Database& database, const Field& field);

        template <typename Model>
        [[nodiscard]]
        CrudResult<void> dropColumn(
            db::Database&      database,
            const std::string& columnName
        );

       private:
        bool _columnExists(
            db::Database&      database,
            const std::string& columnName,
            const std::string& tableName
        );
    };

}   // namespace orm

#ifndef __ORM__INCLUDE__ORM__CRUD_TPP__
#include "orm/crud.tpp"
#endif

#endif   // __ORM__INCLUDE__ORM__CRUD_HPP__
