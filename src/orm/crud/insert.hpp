#ifndef __ORM__CRUD__INSERT_HPP__
#define __ORM__CRUD__INSERT_HPP__

#include <cstdint>
#include <expected>
#include <optional>

#include "crud_error.hpp"
#include "db/database.hpp"
#include "orm/fields.hpp"
#include "orm/type_traits.hpp"

namespace orm
{
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
    [[nodiscard]] std::expected<std::int64_t, CrudError> insert(
        db::Database& database,
        const Model&  row
    )
    {
        const auto fieldViews = orm::fields(row);
        auto       sqlText    = "INSERT INTO " + Model::table_name + " (";
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

        auto statement = database.prepare(sqlText);

        int index = 1;

        for (FieldView const& field : fieldViews)
        {
            if (field.isAutoIncrementPk())
                continue;

            field.bind(statement, index);
            ++index;
        }

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

}   // namespace orm

#endif   // __ORM__CRUD__INSERT_HPP__