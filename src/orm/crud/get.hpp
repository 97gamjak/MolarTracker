#ifndef __ORM__CRUD__GET_HPP__
#define __ORM__CRUD__GET_HPP__

#include <expected>
#include <string>
#include <vector>

#include "crud_detail.hpp"
#include "crud_error.hpp"
#include "db/database.hpp"
#include "mstd/string.hpp"
#include "orm/binder.hpp"
#include "orm/field_view.hpp"
#include "orm/fields.hpp"
#include "orm/type_traits.hpp"

namespace orm
{
    /**
     * @brief Get rows by matching field value
     *
     * @tparam Model
     * @tparam Field
     * @param database
     * @param fieldToMatch
     * @return std::vector<Model>
     */
    template <db_model Model, typename Field>
    [[nodiscard]] std::vector<Model> getByField(
        db::Database& database,
        Field const&  fieldToMatch
    )
    {
        const auto fieldView       = orm::FieldView::from(fieldToMatch);
        auto const emptyFieldViews = orm::fields<Model>();

        std::string sqlText;
        sqlText += "SELECT ";

        const auto columnNames  = getColumnNames(emptyFieldViews);
        sqlText                += mstd::join(columnNames, ", ");

        sqlText += " FROM ";
        sqlText += Model::table_name;
        sqlText += " WHERE ";
        sqlText += std::string{fieldView.getColumnName()};
        sqlText += "=?;";

        db::Statement statement = database.prepare(sqlText);
        fieldView.bind(statement, 1);

        std::vector<Model> results;

        while (statement.step() == db::StepResult::RowAvailable)
        {
            results.push_back(loadModelFromStatement<Model>(statement));
        }

        return results;
    }

    /**
     * @brief Get a single row by matching unique field value
     *
     * @tparam Model
     * @tparam Field
     * @param database
     * @param fieldToMatch
     * @return std::expected<Model, CrudError> The loaded model on success, or
     * an error on failure
     */
    template <db_model Model, typename Field>
    [[nodiscard]] std::expected<Model, CrudError> getByUniqueField(
        db::Database& database,
        Field const&  fieldToMatch
    )
    {
        static_assert(Field::isUnique);
        const auto fieldView = orm::FieldView::from(fieldToMatch);

        auto const emptyFieldViews = orm::fields<Model>();

        std::string sqlText;
        sqlText += "SELECT ";

        const auto columnNames  = getColumnNames(emptyFieldViews);
        sqlText                += mstd::join(columnNames, ", ");

        sqlText += " FROM ";
        sqlText += Model::table_name;
        sqlText += " WHERE ";
        sqlText += std::string{fieldView.getColumnName()};
        sqlText += "=?;";

        db::Statement statement = database.prepare(sqlText);
        fieldView.bind(statement, 1);

        const db::StepResult result = statement.step();
        if (result != db::StepResult::RowAvailable)
        {
            return std::unexpected(CrudError(
                CrudErrorType::NotFound,
                "No row found matching the specified unique field value"
            ));
        }

        return loadModelFromStatement<Model>(statement);
    }

    /**
     * @brief Get a single row by primary key value
     *
     * @tparam Model
     * @tparam PrimaryKeyValue
     * @param database
     * @param pkValue
     * @return std::optional<Model> The loaded model if found, or std::nullopt
     * if not found
     */
    template <db_model Model, typename PrimaryKeyValue>
    [[nodiscard]] std::optional<Model> getByPk(
        db::Database&          database,
        PrimaryKeyValue const& pkValue
    )
    {
        const auto numberOfPkFields = getNumberOfPkFields<Model>();
        if (numberOfPkFields != 1)
        {
            throw CrudException(
                "orm::getByPk requires a model with exactly one primary key "
                "field"
            );
        }

        auto const emptyFieldViews = orm::fields<Model>();

        std::string sqlText;
        sqlText += "SELECT ";

        const auto columnNames = getColumnNames(emptyFieldViews);

        sqlText += mstd::join(columnNames, ", ");

        sqlText += " FROM ";
        sqlText += Model::table_name;
        sqlText += " WHERE ";

        const auto whereClauses = getColumnNames(
            emptyFieldViews,
            ORMConstraint::PrimaryKey,
            ORMConstraintMode::Only,
            "=?"
        );

        if (whereClauses.empty())
        {
            throw CrudException(
                "orm::getByPk requires at least one primary key field"
            );
        }

        sqlText += whereClauses[0];
        sqlText += ";";

        db::Statement statement = database.prepare(sqlText);
        binder<db::Statement, PrimaryKeyValue>::bind(statement, 1, pkValue);

        const db::StepResult result = statement.step();
        if (result != db::StepResult::RowAvailable)
            return std::nullopt;

        return loadModelFromStatement<Model>(statement);
    }

    /**
     * @brief Get all rows from the table
     *
     * @tparam Model
     * @param database
     * @return std::vector<Model>
     */
    template <db_model Model>
    [[nodiscard]] std::vector<Model> getAll(db::Database& database)
    {
        auto const emptyFieldViews = orm::fields<Model>();

        std::string sqlText;
        sqlText += "SELECT ";

        const auto columnNames  = getColumnNames(emptyFieldViews);
        sqlText                += mstd::join(columnNames, ", ");

        sqlText += " FROM ";
        sqlText += Model::table_name;
        sqlText += ";";

        db::Statement statement = database.prepare(sqlText);

        std::vector<Model> results;

        while (statement.step() == db::StepResult::RowAvailable)
        {
            results.push_back(loadModelFromStatement<Model>(statement));
        }

        return results;
    }

}   // namespace orm

#endif   // __ORM__CRUD__GET_HPP__