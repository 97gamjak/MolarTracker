#ifndef __ORM__CRUD_HPP__
#define __ORM__CRUD_HPP__

#include <cstdint>
#include <stdexcept>
#include <string>

#include "db/database.hpp"
#include "db/statement.hpp"
#include "orm/binder.hpp"
#include "orm/fields.hpp"
#include "orm/model_concept.hpp"

namespace orm
{
    struct SQL
    {
        static inline constexpr std::string create_table(
            const std::string& table_name
        )
        {
            return "CREATE TABLE IF NOT EXISTS " + table_name;
        }

        static inline constexpr std::string insert_into(
            const std::string& table_name
        )
        {
            return "INSERT INTO " + table_name;
        }
    };

    template <db_model Model>
    [[nodiscard]] std::string create_table_sql()
    {
        const auto field_views = orm::fields(Model{});
        auto       sql_text    = SQL::create_table(Model::table_name) + " (";
        auto       first_col   = true;

        for (FieldView const& field : field_views)
        {
            if (!first_col)
                sql_text += ", ";

            first_col  = false;
            sql_text  += field.ddl();
        }

        sql_text += ");";
        return sql_text;
    }

    template <db_model Model>
    [[nodiscard]] std::int64_t insert(db::Database& database, Model const& row)
    {
        auto const field_views  = orm::fields(row);
        auto       sql_text     = SQL::insert_into(Model::table_name) + " (";
        auto       first_column = true;

        for (FieldView const& field : field_views)
        {
            if (field.is_auto_increment_pk())
                continue;

            if (!first_column)
                sql_text += ", ";

            first_column  = false;
            sql_text     += std::string{field.column_name()};
        }

        sql_text += ") VALUES (";

        first_column = true;

        for (FieldView const& field : field_views)
        {
            if (field.is_auto_increment_pk())
                continue;

            if (!first_column)
                sql_text += ", ";

            first_column  = false;
            sql_text     += "?";
        }

        sql_text += ");";

        db::Statement statement = database.prepare(sql_text);

        int index = 1;

        for (FieldView const& field : field_views)
        {
            if (field.is_auto_increment_pk())
                continue;

            field.bind(statement, index);
            ++index;
        }

        statement.execute_to_completion();
        return database.last_insert_rowid();
    }

    template <db_model Model>
    void update(db::Database& database, Model const& row)
    {
        auto const field_views = orm::fields(row);

        std::string sql_text;
        sql_text += "UPDATE ";
        sql_text += Model::table_name;
        sql_text += " SET ";

        bool first_assignment = true;

        for (FieldView const& field : field_views)
        {
            if (field.is_pk())
                continue;

            if (!first_assignment)
                sql_text += ", ";

            first_assignment  = false;
            sql_text         += std::string{field.column_name()};
            sql_text         += "=?";
        }

        sql_text += " WHERE ";

        bool where_is_present = false;

        for (FieldView const& field : field_views)
        {
            if (!field.is_pk())
                continue;

            if (!where_is_present)
            {
                where_is_present  = true;
                sql_text         += std::string{field.column_name()};
                sql_text         += "=?";
            }
        }

        if (!where_is_present)
        {
            throw std::runtime_error(
                "orm::update requires a primary key field"
            );
        }

        sql_text += ";";

        db::Statement statement = database.prepare(sql_text);

        int index = 1;

        for (FieldView const& field : field_views)
        {
            if (field.is_pk())
            {
                continue;
            }

            field.bind(statement, index);
            ++index;
        }

        for (FieldView const& field : field_views)
        {
            if (!field.is_pk())
            {
                continue;
            }

            field.bind(statement, index);
        }

        statement.execute_to_completion();
    }

    template <db_model Model, typename Field>
    [[nodiscard]] std::vector<Model> get_by_field(
        db::Database& database,
        Field const&  field_to_match
    )
    {
        const auto field_view = orm::FieldView::from(field_to_match);

        Model      empty_instance{};
        auto const empty_field_views = orm::fields(empty_instance);

        std::string sql_text;
        sql_text += "SELECT ";

        bool first_select_column = true;

        for (FieldView const& field : empty_field_views)
        {
            if (!first_select_column)
            {
                sql_text += ", ";
            }

            first_select_column  = false;
            sql_text            += std::string{field.column_name()};
        }

        sql_text += " FROM ";
        sql_text += Model::table_name;
        sql_text += " WHERE ";
        sql_text += std::string{field_view.column_name()};
        sql_text += "=?;";

        db::Statement statement = database.prepare(sql_text);
        field_view.bind(statement, 1);

        std::vector<Model> results;

        while (statement.step() == db::StepResult::RowAvailable)
        {
            Model      loaded_model{};
            auto const loaded_field_views = orm::fields(loaded_model);

            int col = 0;

            for (FieldView const& field : loaded_field_views)
            {
                field.read_from(statement, col);
                ++col;
            }

            results.push_back(std::move(loaded_model));
        }

        return results;
    }

    template <db_model Model, typename Field>
    [[nodiscard]] std::optional<Model> get_by_unique_field(
        db::Database& database,
        Field const&  field_to_match
    )
    {
        static_assert(Field::is_unique);
        const auto field_view = orm::FieldView::from(field_to_match);

        Model      empty_instance{};
        auto const empty_field_views = orm::fields(empty_instance);

        std::string sql_text;
        sql_text += "SELECT ";

        bool first_select_column = true;

        for (FieldView const& field : empty_field_views)
        {
            if (!first_select_column)
            {
                sql_text += ", ";
            }

            first_select_column  = false;
            sql_text            += std::string{field.column_name()};
        }

        sql_text += " FROM ";
        sql_text += Model::table_name;
        sql_text += " WHERE ";
        sql_text += std::string{field_view.column_name()};
        sql_text += "=?;";

        db::Statement statement = database.prepare(sql_text);
        field_view.bind(statement, 1);

        const db::StepResult result = statement.step();
        if (result != db::StepResult::RowAvailable)
        {
            return std::nullopt;
        }

        Model      loaded_model{};
        auto const loaded_field_views = orm::fields(loaded_model);

        int col = 0;

        for (FieldView const& field : loaded_field_views)
        {
            field.read_from(statement, col);
            ++col;
        }

        return loaded_model;
    }

    template <db_model Model, typename PrimaryKeyValue>
    [[nodiscard]] std::optional<Model> get_by_pk(
        db::Database&          database,
        PrimaryKeyValue const& pk_value
    )
    {
        Model      empty_instance{};
        auto const empty_field_views = orm::fields(empty_instance);

        std::string sql_text;
        sql_text += "SELECT ";

        bool first_select_column = true;

        for (FieldView const& field : empty_field_views)
        {
            if (!first_select_column)
            {
                sql_text += ", ";
            }

            first_select_column  = false;
            sql_text            += std::string{field.column_name()};
        }

        sql_text += " FROM ";
        sql_text += Model::table_name;
        sql_text += " WHERE ";

        bool where_is_present = false;

        for (FieldView const& field : empty_field_views)
        {
            if (!field.is_pk())
            {
                continue;
            }

            if (!where_is_present)
            {
                where_is_present  = true;
                sql_text         += std::string{field.column_name()};
                sql_text         += "=?";
            }
        }

        if (!where_is_present)
        {
            throw std::runtime_error(
                "orm::select_by_pk requires a primary key field"
            );
        }

        sql_text += ";";

        db::Statement statement = database.prepare(sql_text);
        orm::binder<db::Statement, PrimaryKeyValue>::bind(
            statement,
            1,
            pk_value
        );

        const db::StepResult result = statement.step();
        if (result != db::StepResult::RowAvailable)
        {
            return std::nullopt;
        }

        Model      loaded_model{};
        auto const loaded_field_views = orm::fields(loaded_model);

        int col = 0;

        for (FieldView const& field : loaded_field_views)
        {
            field.read_from(statement, col);
            ++col;
        }

        return loaded_model;
    }

    template <db_model Model>
    [[nodiscard]] std::vector<Model> get_all(db::Database& database)
    {
        Model      empty_instance{};
        auto const empty_field_views = orm::fields(empty_instance);

        std::string sql_text;
        sql_text += "SELECT ";

        bool first_select_column = true;

        for (FieldView const& field : empty_field_views)
        {
            if (!first_select_column)
            {
                sql_text += ", ";
            }

            first_select_column  = false;
            sql_text            += std::string{field.column_name()};
        }

        sql_text += " FROM ";
        sql_text += Model::table_name;
        sql_text += ";";

        db::Statement statement = database.prepare(sql_text);

        std::vector<Model> results;

        while (statement.step() == db::StepResult::RowAvailable)
        {
            Model      loaded_model{};
            auto const loaded_field_views = orm::fields(loaded_model);

            int col = 0;

            for (FieldView const& field : loaded_field_views)
            {
                field.read_from(statement, col);
                ++col;
            }

            results.push_back(std::move(loaded_model));
        }

        return results;
    }

}   // namespace orm

#endif   // __ORM__CRUD_HPP__