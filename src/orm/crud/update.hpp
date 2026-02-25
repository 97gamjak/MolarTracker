#ifndef __ORM__CRUD__UPDATE_HPP__
#define __ORM__CRUD__UPDATE_HPP__

#include <string>
#include <vector>

#include "db/database.hpp"
#include "mstd/enum.hpp"
#include "orm/constraints.hpp"
#include "orm/fields.hpp"
#include "orm/orm_exception.hpp"
#include "orm/type_traits.hpp"
#include "utils/string.hpp"

namespace orm
{

    // NOLINTBEGIN
#define ORM_CONSTRAINT_MODE(X) \
    X(All)                     \
    X(Not)                     \
    X(Only)

    MSTD_ENUM(ORMConstraintMode, uint8_t, ORM_CONSTRAINT_MODE);
    // NOLINTEND

    template <typename FieldRange>
    std::vector<std::string> getColumnNames(
        const FieldRange& fields,
        ORMConstraint     constraint,
        ORMConstraintMode mode
    )
    {
        return getColumnNames(fields, constraint, mode, "");
    }

    template <typename FieldRange>
    std::vector<std::string> getColumnNames(
        const FieldRange&  fields,
        ORMConstraint      constraint,
        ORMConstraintMode  mode,
        const std::string& suffix
    )
    {
        std::vector<std::string> sqlTexts;

        for (const auto& field : fields)
        {
            if (mode == ORMConstraintMode::Only)
            {
                if ((field.getConstraints() & constraint) == constraint)
                    continue;
            }
            else if (mode == ORMConstraintMode::Not)
            {
                if ((field.getConstraints() & constraint) != constraint)
                    continue;
            }

            sqlTexts.push_back(std::string{field.getColumnName()} + suffix);
        }

        return sqlTexts;
    }

    template <typename FieldRange>
    void bindFieldsToStatement(
        db::Statement&    statement,
        const FieldRange& fields,
        ORMConstraint     constraint,
        ORMConstraintMode mode
    )
    {
        int index = 1;

        for (const auto& field : fields)
        {
            if (mode == ORMConstraintMode::Only)
            {
                if ((field.getConstraints() & constraint) == constraint)
                    continue;
            }
            else if (mode == ORMConstraintMode::Not)
            {
                if ((field.getConstraints() & constraint) != constraint)
                    continue;
            }

            field.bind(statement, index);
            ++index;
        }
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

        const auto columnNames = getColumnNamesBinder(
            fieldViews,
            ORMConstraint::PrimaryKey,
            ORMConstraintMode::Not
        );

        sqlText += utils::join(columnNames, ", ");
        sqlText += " WHERE ";

        const auto whereClauses = getColumnNamesBinder(
            fieldViews,
            ORMConstraint::PrimaryKey,
            ORMConstraintMode::Only,
            " = ?"
        );

        if (whereClauses.empty())
        {
            throw ORMError(
                "orm::update requires at least one primary key field"
            );
        }

        sqlText += utils::join(whereClauses, " AND ");
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