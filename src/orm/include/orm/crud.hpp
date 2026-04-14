#ifndef __ORM__INCLUDE__ORM__CRUD_HPP__
#define __ORM__INCLUDE__ORM__CRUD_HPP__

#include <cstdint>
#include <expected>
#include <mstd/error.hpp>
#include <optional>
#include <vector>

#include "crud/crud_error.hpp"
#include "db/database.hpp"
#include "db/transaction.hpp"
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
        void createTable(db::Database& database);

        template <db_model Model>
        void createTable(db::Database& database, std::string_view tableName);

        /******************
         * INSERT METHODS *
         ******************/

        template <db_model Model>
        [[nodiscard]] std::expected<std::int64_t, CrudError> insert(
            db::Database& database,
            const Model&  row
        );

        template <db_model Model>
        [[nodiscard]] std::expected<std::int64_t, CrudError> insert(
            db::Database& database,
            const db::Transaction& /*transaction*/,
            const Model& row
        );

        template <typename... Models>
        requires(db_model<Models> && ...)
        [[nodiscard]] std::expected<std::vector<std::int64_t>, CrudError> batchInsert(
            db::Database& database,
            const Models&... rows
        );

        /******************
         * UPDATE METHODS *
         ******************/

        template <db_model Model>
        [[nodiscard]] std::expected<void, CrudError> update(
            db::Database& database,
            const Model&  row
        );

        template <typename Field>
        [[nodiscard]] std::expected<void, CrudError> updateField(
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

        template <db_model... Models>
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

        /***************
         * ADD METHODS *
         ***************/

        template <typename Field>
        std::expected<void, CrudError> addColumn(
            db::Database& database,
            const Field&  field
        );

       private:
        template <typename Field>
        bool _columnExists(db::Database& database);
    };

}   // namespace orm

#ifndef __ORM__INCLUDE__ORM__CRUD_TPP__
#include "orm/crud.tpp"
#endif

#endif   // __ORM__INCLUDE__ORM__CRUD_HPP__
