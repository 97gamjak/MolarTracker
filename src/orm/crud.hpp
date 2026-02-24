#ifndef __ORM__CRUD_HPP__
#define __ORM__CRUD_HPP__

#include <cstdint>
#include <string>
#include <vector>

#include "crud/create.hpp"   // IWYU pragma: export
#include "db/database.hpp"
#include "db/statement.hpp"
#include "orm/binder.hpp"
#include "orm/fields.hpp"
#include "orm/type_traits.hpp"
#include "orm_exception.hpp"

namespace orm
{
    /**
     * @brief SQL statement helpers
     *
     */
    struct SQL
    {
        /**
         * @brief Insert into SQL statement
         *
         * @param table_name
         * @return constexpr std::string
         */
        static constexpr std::string insert_into(const std::string& table_name)
        {
            return "INSERT INTO " + table_name;
        }
    };

    /**
     * @brief Insert a row into the database
     *
     * @tparam Model
     * @param database
     * @param row
     * @return std::int64_t The last inserted row ID
     */
    template <db_model Model>
    [[nodiscard]] std::int64_t insert(db::Database& database, Model const& row)
    {
        auto const fieldViews = orm::fields(row);
        auto       sqlText    = SQL::insert_into(Model::table_name) + " (";
        auto       firstCol   = true;

        for (FieldView const& field : fieldViews)
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

        db::Statement statement = database.prepare(sqlText);

        int index = 1;

        for (FieldView const& field : fieldViews)
        {
            if (field.isAutoIncrementPk())
                continue;

            field.bind(statement, index);
            ++index;
        }

        statement.executeToCompletion();
        return database.lastInsertRowid();
    }

    /**
     * @brief Update a row in the database
     *
     * @tparam Model
     * @param database
     * @param row
     */
    template <db_model Model>
    void update(db::Database& database, Model const& row)
    {
        auto const fieldViews = orm::fields(row);

        std::string sqlText;
        sqlText += "UPDATE ";
        sqlText += Model::table_name;
        sqlText += " SET ";

        bool firstAssignment = true;

        for (FieldView const& field : fieldViews)
        {
            if (field.isPk())
                continue;

            if (!firstAssignment)
                sqlText += ", ";

            firstAssignment  = false;
            sqlText         += std::string{field.getColumnName()};
            sqlText         += "=?";
        }

        sqlText += " WHERE ";

        bool whereIsPresent = false;

        for (FieldView const& field : fieldViews)
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
            throw ORMError(
                "orm::update requires at least one primary key field"
            );
        }

        sqlText += ";";

        db::Statement statement = database.prepare(sqlText);

        int index = 1;

        for (FieldView const& field : fieldViews)
        {
            if (field.isPk())
                continue;

            field.bind(statement, index);
            ++index;
        }

        for (FieldView const& field : fieldViews)
        {
            if (!field.isPk())
                continue;

            field.bind(statement, index);
        }

        statement.executeToCompletion();
    }

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

#endif   // __ORM__CRUD_HPP__