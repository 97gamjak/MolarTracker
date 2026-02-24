#ifndef __ORM__CRUD_HPP__
#define __ORM__CRUD_HPP__

#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "db/database.hpp"
#include "db/statement.hpp"
#include "orm/binder.hpp"
#include "orm/fields.hpp"
#include "orm/model_concept.hpp"
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
         * @brief Create table SQL statement
         *
         * @param table_name
         * @return constexpr std::string
         */
        static constexpr std::string create_table(const std::string& table_name)
        {
            return "CREATE TABLE IF NOT EXISTS " + table_name;
        }

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

    template <typename Model, typename Group, std::size_t... I>
    void append_unique_group_sql(
        std::string& sql,
        Group const& group,
        std::index_sequence<I...>
    )
    {
        // Optional constraint name (stable & readable)
        sql += ", CONSTRAINT uq_";
        sql += std::string(Model::table_name);

        ((sql += "_", sql += std::string(std::get<I>(group).getColumnName())),
         ...);

        sql        += " UNIQUE (";
        bool first  = true;
        ((sql += (first ? (first = false, "") : ", "),
          sql += std::string(std::get<I>(group).getColumnName())),
         ...);
        sql += ")";
    }

    template <typename Model, typename Group>
    void append_unique_group_sql(std::string& sql, Group const& group)
    {
        constexpr std::size_t n =
            std::tuple_size_v<std::remove_reference_t<Group>>;
        append_unique_group_sql<Model>(
            sql,
            group,
            std::make_index_sequence<n>{}
        );
    }

    template <typename Model, typename Groups, std::size_t... I>
    void append_all_unique_groups_sql(
        std::string&  sql,
        Groups const& groups,
        std::index_sequence<I...>
    )
    {
        (append_unique_group_sql<Model>(sql, std::get<I>(groups)), ...);
    }

    /**
     * @brief Create a table for the specified model in the database
     *
     * @tparam Model
     * @param database
     */
    template <db_model Model>
    void create_table(db::Database& database)
    {
        const auto fieldViews = orm::fields(Model{});
        auto       sqlText    = SQL::create_table(Model::table_name) + " (";
        auto       firstCol   = true;

        for (FieldView const& field : fieldViews)
        {
            if (!firstCol)
                sqlText += ", ";

            firstCol  = false;
            sqlText  += field.ddl();
        }

        if constexpr (has_unique_groups<Model>)
        {
            auto const            groups = Model::getUniqueGroups();
            constexpr std::size_t m      = std::tuple_size_v<decltype(groups)>;
            append_all_unique_groups_sql<Model>(
                sqlText,
                groups,
                std::make_index_sequence<m>{}
            );
        }

        sqlText += ");";

        database.execute(sqlText);
    }

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