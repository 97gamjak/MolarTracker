#ifndef __ORM__CRUD_HPP__
#define __ORM__CRUD_HPP__

#include <cstdint>
#include <expected>
#include <optional>
#include <vector>

#include "db/database.hpp"
#include "orm/crud/crud_error.hpp"
#include "orm/type_traits.hpp"

namespace orm
{

    template <db_model Model>
    void createTable(db::Database& database);

    template <db_model Model>
    [[nodiscard]] std::expected<std::int64_t, CrudError> insert(
        db::Database& database,
        const Model&  row
    );

    template <db_model Model>
    [[nodiscard]] std::expected<void, CrudError> update(
        db::Database& database,
        const Model&  row
    );

    template <db_model Model, typename Field>
    [[nodiscard]] std::vector<Model> getByField(
        db::Database& database,
        Field const&  fieldToMatch
    );

    template <db_model Model, typename Field>
    [[nodiscard]] std::expected<Model, CrudError> getByUniqueField(
        db::Database& database,
        Field const&  fieldToMatch
    );

    template <db_model Model, typename PrimaryKeyValue>
    [[nodiscard]] std::optional<Model> getByPk(
        db::Database&          database,
        PrimaryKeyValue const& pkValue
    );

    template <db_model Model>
    [[nodiscard]] std::vector<Model> getAll(db::Database& database);

    template <db_model Model, typename PrimaryKeyValue>
    void deleteByPk(db::Database& database, PrimaryKeyValue const& pkValue);

}   // namespace orm

#include "crud/create.hpp"   // IWYU pragma: keep
#include "crud/delete.hpp"   // IWYU pragma: keep
#include "crud/get.hpp"      // IWYU pragma: keep
#include "crud/insert.hpp"   // IWYU pragma: keep
#include "crud/update.hpp"   // IWYU pragma: keep

#endif   // __ORM__CRUD_HPP__