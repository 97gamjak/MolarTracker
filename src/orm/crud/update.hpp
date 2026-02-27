#ifndef __ORM__CRUD__UPDATE_HPP__
#define __ORM__CRUD__UPDATE_HPP__

#include <expected>
#include <string>

#include "crud_detail.hpp"
#include "crud_error.hpp"
#include "db/database.hpp"
#include "mstd/string.hpp"
#include "orm/constraints.hpp"
#include "orm/fields.hpp"
#include "orm/type_traits.hpp"

namespace orm
{

    /**
     * @brief Update a row in the database
     *
     * @tparam Model
     * @param database
     * @param row
     * @return std::expected<void, CrudError> An empty expected on success, or
     * an error on failure
     */
    template <db_model Model>
    std::expected<void, CrudError> update(
        db::Database& database,
        const Model&  row
    )
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

        sqlText += mstd::join(columnNames, ", ");
        sqlText += " WHERE ";

        const auto whereClauses = getColumnNames(
            fieldViews,
            ORMConstraint::PrimaryKey,
            ORMConstraintMode::Only,
            " = ?"
        );

        if (whereClauses.empty())
        {
            return std::unexpected(CrudError(
                CrudErrorType::NoPrimaryKey,
                "orm::update requires at least one primary key field"
            ));
        }

        sqlText += mstd::join(whereClauses, " AND ");
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
            return std::unexpected(CrudError(
                CrudErrorType::NoRowsUpdated,
                "orm::update did not update any rows. This may be because the "
                "primary key value(s) did not match any existing row."
            ));
        }

        if (changes > 1)
        {
            return std::unexpected(CrudError(
                CrudErrorType::MultipleRowsUpdated,
                "orm::update updated multiple rows. This should never happen, "
                "as updates are performed by matching primary key values."
            ));
        }

        return {};
    }
}   // namespace orm

#endif   // __ORM__CRUD__UPDATE_HPP__