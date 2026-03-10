#ifndef __ORM__CRUD_HPP__
#define __ORM__CRUD_HPP__

#include <cstdint>
#include <expected>
#include <mstd/error.hpp>
#include <optional>
#include <vector>

#include "constraints.hpp"
#include "crud/crud_error.hpp"
#include "db/database.hpp"
#include "db/transaction.hpp"
#include "type_traits.hpp"

namespace orm
{
    /******************
     * CREATE METHODS *
     ******************/

    template <db_model Model>
    void createTable(const std::shared_ptr<db::Database>& database);

    /******************
     * INSERT METHODS *
     ******************/

    template <db_model Model>
    requires is_freely_insertable_v<Model>
    [[nodiscard]] std::expected<std::int64_t, CrudError> insert(
        const std::shared_ptr<db::Database>& database,
        const Model&                         row
    );

    template <db_model Model>
    [[nodiscard]] std::expected<std::int64_t, CrudError> insert(
        const std::shared_ptr<db::Database>& database,
        const db::Transaction&               transaction,
        const Model&                         row
    );

    template <typename... Models>
    requires(db_model<Models> && ...)
    [[nodiscard]] std::expected<std::vector<std::int64_t>, CrudError> batchInsert(
        const std::shared_ptr<db::Database>& database,
        const Models&... rows
    );

    /******************
     * UPDATE METHODS *
     ******************/

    template <db_model Model>
    [[nodiscard]] std::expected<void, CrudError> update(
        const std::shared_ptr<db::Database>& database,
        const Model&                         row
    );

    /***************
     * GET METHODS *
     ***************/

    template <db_model Model, typename Field>
    [[nodiscard]] std::vector<Model> getByField(
        const std::shared_ptr<db::Database>& database,
        Field const&                         fieldToMatch
    );

    template <db_model Model, typename Field>
    [[nodiscard]] std::expected<Model, CrudError> getByUniqueField(
        const std::shared_ptr<db::Database>& database,
        Field const&                         fieldToMatch
    );

    template <db_model Model, typename PrimaryKeyValue>
    [[nodiscard]] std::optional<Model> getByPk(
        const std::shared_ptr<db::Database>& database,
        PrimaryKeyValue const&               pkValue
    );

    template <db_model Model>
    [[nodiscard]] std::vector<Model> getAll(db::Database& database);

    template <db_model Model, typename PrimaryKeyValue>
    void deleteByPk(
        const std::shared_ptr<db::Database>& database,
        PrimaryKeyValue const&               pkValue
    );

}   // namespace orm

#include "crud/create.hpp"   // IWYU pragma: export
#include "crud/delete.hpp"   // IWYU pragma: export
#include "crud/get.hpp"      // IWYU pragma: export
#include "crud/insert.hpp"   // IWYU pragma: export
#include "crud/update.hpp"   // IWYU pragma: export

#endif   // __ORM__CRUD_HPP__