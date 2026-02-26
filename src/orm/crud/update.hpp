#ifndef __ORM__CRUD__UPDATE_HPP__
#define __ORM__CRUD__UPDATE_HPP__

#include <string>

#include "crud_detail.hpp"
#include "db/database.hpp"
#include "orm/constraints.hpp"
#include "orm/fields.hpp"
#include "orm/orm_exception.hpp"
#include "orm/type_traits.hpp"
#include "utils/string.hpp"

namespace orm
{

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

        const auto columnNames = getColumnNames(
            fieldViews,
            ORMConstraint::PrimaryKey,
            ORMConstraintMode::Not
        );

        sqlText += utils::join(columnNames, ", ");
        sqlText += " WHERE ";

        const auto whereClauses = getColumnNames(
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

        bindFieldsToStatement(
            statement,
            fieldViews,
            ORMConstraint::PrimaryKey,
            ORMConstraintMode::Not
        );
        bindFieldsToStatement(
            statement,
            fieldViews,
            ORMConstraint::PrimaryKey,
            ORMConstraintMode::Only
        );

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