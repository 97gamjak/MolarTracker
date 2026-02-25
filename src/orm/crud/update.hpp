#ifndef __ORM__CRUD__UPDATE_HPP__
#define __ORM__CRUD__UPDATE_HPP__

#include <ranges>
#include <string>
#include <vector>

#include "db/database.hpp"
#include "orm/fields.hpp"
#include "orm/orm_exception.hpp"
#include "orm/type_traits.hpp"

namespace orm
{
    template <typename FieldRange>
    std::vector<std::string> getColumnNamesBinder(
        const FieldRange& fields,
        bool              usePrimaryKey
    )
    {
        std::vector<std::string> sqlTexts;

        for (const auto& field : fields)
        {
            if (usePrimaryKey != field.isPk())
                continue;

            sqlTexts.push_back(std::string{field.getColumnName()} + "=?");
        }

        return sqlTexts;
    }

    template <typename FieldRange>
    void bindFieldsToStatement(
        db::Statement&    statement,
        const FieldRange& fields,
        bool              bindPrimaryKey
    )
    {
        int index = 1;

        for (const auto& field : fields)
        {
            if (bindPrimaryKey != field.isPk())
                continue;

            field.bind(statement, index);
            ++index;
        }
    }

    std::string concat(
        const std::vector<std::string>& vector,
        const std::string&              delimiter
    )
    {
        auto joined = std::ranges::views::join_with(vector, delimiter);
        return {joined.begin(), joined.end()};
    }

    /**
     * @brief Update a row in the database
     *
     * @tparam Model
     * @param database
     * @param row
     */
    template <db_model Model>
    void update(db::Database& database, const Model& row)
    {
        auto const fieldViews = orm::fields(row);

        std::string sqlText;
        sqlText += "UPDATE ";
        sqlText += Model::table_name;
        sqlText += " SET ";

        auto firstAssignment = true;

        sqlText += concat(getColumnNamesBinder(fieldViews, false), ", ");
        sqlText += " WHERE ";

        const auto whereClauses = getColumnNamesBinder(fieldViews, true);

        if (whereClauses.empty())
        {
            throw ORMError(
                "orm::update requires at least one primary key field"
            );
        }

        sqlText += concat(whereClauses, " AND ");
        sqlText += ";";

        db::Statement statement = database.prepare(sqlText);

        bindFieldsToStatement(statement, fieldViews, false);
        bindFieldsToStatement(statement, fieldViews, true);

        statement.executeToCompletion();

        const auto changes = database.getNumberOfLastChanges();

        if (changes == 0)
        {
            throw ORMError(
                "orm::update did not update any rows. This may be because the "
                "primary key value(s) did not match any existing row."
            );
        }

        if (changes > 1)
        {
            throw ORMError(
                "orm::update updated multiple rows. This should never happen, "
                "as updates are performed by matching primary key values."
            );
        }
    }
}   // namespace orm

#endif   // __ORM__CRUD__UPDATE_HPP__