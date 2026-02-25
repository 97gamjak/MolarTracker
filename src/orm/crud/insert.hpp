#ifndef __ORM__CRUD__INSERT_HPP__
#define __ORM__CRUD__INSERT_HPP__

#include <cstdint>
#include <optional>

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
     * @return std::int64_t The last inserted row ID
     */
    template <db_model Model>
    [[nodiscard]] std::optional<std::int64_t> insert(
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
        return database.getLastInsertRowid();
    }

}   // namespace orm

#endif   // __ORM__CRUD__INSERT_HPP__