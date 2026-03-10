#ifndef __ORM__CRUD__INSERT_HPP__
#define __ORM__CRUD__INSERT_HPP__

#include <cstdint>
#include <expected>
#include <optional>
#include <vector>

#include "crud_error.hpp"
#include "db/database.hpp"
#include "db/transaction.hpp"
#include "orm/constraints.hpp"
#include "orm/fields.hpp"
#include "orm/type_traits.hpp"

namespace orm
{
    namespace details
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
            const std::shared_ptr<db::Database>& database,
            const Model&                         row
        )
        {
            const auto fieldViews = orm::fields(row);
            auto       sqlText    = "INSERT INTO " + Model::tableName + " (";
            auto       firstCol   = true;

            for (const auto& field : fieldViews)
            {
                if (field.isAutoIncrementPk())
                    continue;

                if (!firstCol)
                    sqlText += ", ";

                firstCol  = false;
                sqlText  += std::string{field.getColumnName()};
            }

            sqlText += ") VALUES (";

            firstCol = true;

            for (FieldView const& field : fieldViews)
            {
                if (field.isAutoIncrementPk())
                    continue;

                if (!firstCol)
                    sqlText += ", ";

                firstCol  = false;
                sqlText  += "?";
            }

            sqlText += ");";

            if (database == nullptr)
                throw CrudException("Database pointer is null");

            auto statement = database->prepare(sqlText);

            int index = 1;

            for (FieldView const& field : fieldViews)
            {
                if (field.isAutoIncrementPk())
                    continue;

                field.bind(statement, index);
                ++index;
            }

            statement.executeToCompletion();

            const auto lastInsertId = database->getLastInsertRowid();
            if (lastInsertId.has_value())
                return lastInsertId.value();

            const auto error = CrudError{
                CrudErrorType::InsertFailed,
                "Failed to retrieve last insert ID after insert operation"
            };

            return std::unexpected(error);
        }
    }   // namespace details

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
        // here the passed transaction is used only to enforce that the caller
        // has an active transaction, the actual transaction object is not used
        // because the details::_insert method will create save points for
        // nested transactions if needed, so we can just call it directly
        // without worrying about the transaction management here

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
        if (database == nullptr)
            throw CrudException("Database pointer is null");

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

}   // namespace orm

#endif   // __ORM__CRUD__INSERT_HPP__