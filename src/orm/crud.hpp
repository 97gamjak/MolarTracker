#ifndef __ORM__CRUD_HPP__
#define __ORM__CRUD_HPP__

#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <vector>

#include "db/database.hpp"
#include "db/statement.hpp"
#include "orm/binder.hpp"
#include "orm/crud/crud_error.hpp"
#include "orm/fields.hpp"
#include "orm/type_traits.hpp"
#include "orm_exception.hpp"

namespace orm
{

    template <db_model Model>
    void create_table(db::Database& database);

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
    [[nodiscard]] std::vector<Model> get_by_field(
        db::Database& database,
        Field const&  fieldToMatch
    )
    {
        const auto fieldView = orm::FieldView::from(fieldToMatch);

        Model      emptyInstance{};
        auto const emptyFieldViews = orm::fields(emptyInstance);

        std::string sqlText;
        sqlText += "SELECT ";

        bool firstSelectColumn = true;

        for (FieldView const& field : emptyFieldViews)
        {
            if (!firstSelectColumn)
                sqlText += ", ";

            firstSelectColumn  = false;
            sqlText           += std::string{field.getColumnName()};
        }

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
            Model      loadedModel{};
            auto const loadedFieldViews = orm::fields(loadedModel);

            int col = 0;

            for (FieldView const& field : loadedFieldViews)
            {
                field.readFrom(statement, col);
                ++col;
            }

            results.push_back(std::move(loadedModel));
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
     * @return std::optional<Model>
     */
    template <db_model Model, typename Field>
    [[nodiscard]] std::optional<Model> get_by_unique_field(
        db::Database& database,
        Field const&  fieldToMatch
    )
    {
        static_assert(Field::isUnique);
        const auto fieldView = orm::FieldView::from(fieldToMatch);

        Model      emptyInstance{};
        auto const emptyFieldViews = orm::fields(emptyInstance);

        std::string sqlText;
        sqlText += "SELECT ";

        bool firstSelectColumn = true;

        for (FieldView const& field : emptyFieldViews)
        {
            if (!firstSelectColumn)
                sqlText += ", ";

            firstSelectColumn  = false;
            sqlText           += std::string{field.getColumnName()};
        }

        sqlText += " FROM ";
        sqlText += Model::table_name;
        sqlText += " WHERE ";
        sqlText += std::string{fieldView.getColumnName()};
        sqlText += "=?;";

        db::Statement statement = database.prepare(sqlText);
        fieldView.bind(statement, 1);

        const db::StepResult result = statement.step();
        if (result != db::StepResult::RowAvailable)
            return std::nullopt;

        Model      loadedModel{};
        auto const loadedFieldViews = orm::fields(loadedModel);

        int col = 0;

        for (FieldView const& field : loadedFieldViews)
        {
            field.readFrom(statement, col);
            ++col;
        }

        return loadedModel;
    }

    template <db_model Model, typename PrimaryKeyValue>
    [[nodiscard]] std::optional<Model> get_by_pk(
        db::Database&          database,
        PrimaryKeyValue const& pk_value
    )
    {
        Model      emptyInstance{};
        auto const emptyFieldViews = orm::fields(emptyInstance);

        std::string sqlText;
        sqlText += "SELECT ";

        bool firstSelectColumn = true;

        for (FieldView const& field : emptyFieldViews)
        {
            if (!firstSelectColumn)
                sqlText += ", ";

            firstSelectColumn  = false;
            sqlText           += std::string{field.getColumnName()};
        }

        sqlText += " FROM ";
        sqlText += Model::table_name;
        sqlText += " WHERE ";

        bool whereIsPresent = false;

        for (FieldView const& field : emptyFieldViews)
        {
            if (!field.isPk())
                continue;

            if (!whereIsPresent)
            {
                whereIsPresent  = true;
                sqlText        += std::string{field.getColumnName()};
                sqlText        += "=?";
            }
        }

        if (!whereIsPresent)
        {
            throw ORMError("orm::select_by_pk requires a primary key field");
        }

        sqlText += ";";

        db::Statement statement = database.prepare(sqlText);
        orm::binder<db::Statement, PrimaryKeyValue>::bind(
            statement,
            1,
            pk_value
        );

        const db::StepResult result = statement.step();
        if (result != db::StepResult::RowAvailable)
            return std::nullopt;

        Model      loadedModel{};
        auto const loadedFieldViews = orm::fields(loadedModel);

        int col = 0;

        for (FieldView const& field : loadedFieldViews)
        {
            field.readFrom(statement, col);
            ++col;
        }

        return loadedModel;
    }

    /**
     * @brief Get all rows from the table
     *
     * @tparam Model
     * @param database
     * @return std::vector<Model>
     */
    template <db_model Model>
    [[nodiscard]] std::vector<Model> get_all(db::Database& database)
    {
        Model      emptyInstance{};
        auto const emptyFieldViews = orm::fields(emptyInstance);

        std::string sqlText;
        sqlText += "SELECT ";

        bool firstSelectColumn = true;

        for (FieldView const& field : emptyFieldViews)
        {
            if (!firstSelectColumn)
                sqlText += ", ";

            firstSelectColumn  = false;
            sqlText           += std::string{field.getColumnName()};
        }

        sqlText += " FROM ";
        sqlText += Model::table_name;
        sqlText += ";";

        db::Statement statement = database.prepare(sqlText);

        std::vector<Model> results;

        while (statement.step() == db::StepResult::RowAvailable)
        {
            Model      loadedModel{};
            auto const loadedFieldViews = orm::fields(loadedModel);

            int col = 0;

            for (FieldView const& field : loadedFieldViews)
            {
                field.readFrom(statement, col);
                ++col;
            }

            results.push_back(std::move(loadedModel));
        }

        return results;
    }

    /**
     * @brief Delete a row by primary key value
     *
     * @tparam Model
     * @tparam PrimaryKeyValue
     * @param database
     * @param pk_value
     */
    template <db_model Model, typename PrimaryKeyValue>
    void delete_by_pk(db::Database& database, PrimaryKeyValue const& pk_value)
    {
        Model      emptyInstance{};
        auto const emptyFieldViews = orm::fields(emptyInstance);

        std::string sqlText;
        sqlText += "DELETE FROM ";
        sqlText += Model::table_name;
        sqlText += " WHERE ";

        bool whereIsPresent = false;

        for (FieldView const& field : emptyFieldViews)
        {
            if (!field.isPk())
                continue;

            if (!whereIsPresent)
            {
                whereIsPresent  = true;
                sqlText        += std::string{field.getColumnName()};
                sqlText        += "=?";
            }
        }

        if (!whereIsPresent)
        {
            throw ORMError("orm::delete_by_pk requires a primary key field");
        }

        sqlText                 += ";";
        db::Statement statement  = database.prepare(sqlText);
        orm::binder<db::Statement, PrimaryKeyValue>::bind(
            statement,
            1,
            pk_value
        );

        statement.executeToCompletion();
    }

}   // namespace orm

#include "crud/create.hpp"   // IWYU pragma: keep
#include "crud/insert.hpp"   // IWYU pragma: keep
#include "crud/update.hpp"   // IWYU pragma: keep

#endif   // __ORM__CRUD_HPP__