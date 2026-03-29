#ifndef __ORM__INCLUDE__ORM__CRUD__INSERT_HPP__
#define __ORM__INCLUDE__ORM__CRUD__INSERT_HPP__

#include <cstdint>
#include <expected>
#include <mstd/string.hpp>
#include <optional>
#include <vector>

#include "crud_error.hpp"
#include "db/database.hpp"
#include "db/transaction.hpp"
#include "logging/log_macros.hpp"
#include "orm/index.hpp"
#include "orm/type_traits.hpp"

REGISTER_LOG_CATEGORY("Orm.Crud.Insert");

namespace orm::details
{
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
    [[nodiscard]] std::expected<std::int64_t, CrudError> _insert(
        db::Database& database,
        const Model&  row
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
     * @tparam Model1
     * @tparam Models
     */
    template <typename... Models>
    requires(db_model<Models> && ...)
    [[nodiscard]] std::expected<std::vector<std::int64_t>, CrudError> batchInsert(
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

                auto result = details::_insert(database, row);
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

}   // namespace orm::details

#endif   // __ORM__INCLUDE__ORM__CRUD__INSERT_HPP__