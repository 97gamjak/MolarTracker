#ifndef __ORM__CRUD_HPP__
#define __ORM__CRUD_HPP__

#include <cstdint>
#include <stdexcept>
#include <string>

#include "binder.hpp"
#include "database.hpp"
#include "fields.hpp"
#include "model_concept.hpp"
#include "statement.hpp"

namespace orm
{
    template <db_model Model>
    [[nodiscard]] std::string create_table_sql()
    {
        Model      model_instance{};
        auto const field_views = orm::fields(model_instance);

        std::string sql_statement_text;
        sql_statement_text += "CREATE TABLE IF NOT EXISTS ";
        sql_statement_text += std::string{Model::table_name.view()};
        sql_statement_text += " (";

        bool first_column_definition = true;

        for (FieldView const& field : field_views)
        {
            if (!first_column_definition)
            {
                sql_statement_text += ", ";
            }

            first_column_definition  = false;
            sql_statement_text      += field.ddl();
        }

        sql_statement_text += ");";
        return sql_statement_text;
    }

    template <db_model Model>
    [[nodiscard]] std::int64_t insert(db::Database& database, Model const& row)
    {
        auto const field_views = orm::fields(row);

        std::string sql_statement_text;
        sql_statement_text += "INSERT INTO ";
        sql_statement_text += std::string{Model::table_name.view()};
        sql_statement_text += " (";

        bool first_column = true;

        for (FieldView const& field : field_views)
        {
            if (field.is_auto_increment_primary_key())
            {
                continue;
            }

            if (!first_column)
            {
                sql_statement_text += ", ";
            }

            first_column        = false;
            sql_statement_text += std::string{field.column_name()};
        }

        sql_statement_text += ") VALUES (";

        bool first_placeholder = true;

        for (FieldView const& field : field_views)
        {
            if (field.is_auto_increment_primary_key())
            {
                continue;
            }

            if (!first_placeholder)
            {
                sql_statement_text += ", ";
            }

            first_placeholder   = false;
            sql_statement_text += "?";
        }

        sql_statement_text += ");";

        db::Statement statement = database.prepare(sql_statement_text);

        int parameter_index_one_based = 1;

        for (FieldView const& field : field_views)
        {
            if (field.is_auto_increment_primary_key())
            {
                continue;
            }

            field.bind(statement, parameter_index_one_based);
            ++parameter_index_one_based;
        }

        statement.execute_to_completion();
        return database.last_insert_rowid();
    }

    template <db_model Model>
    void update(db::Database& database, Model const& row)
    {
        auto const field_views = orm::fields(row);

        std::string sql_statement_text;
        sql_statement_text += "UPDATE ";
        sql_statement_text += std::string{Model::table_name.view()};
        sql_statement_text += " SET ";

        bool first_assignment = true;

        for (FieldView const& field : field_views)
        {
            if (field.is_primary_key())
            {
                continue;
            }

            if (!first_assignment)
            {
                sql_statement_text += ", ";
            }

            first_assignment    = false;
            sql_statement_text += std::string{field.column_name()};
            sql_statement_text += "=?";
        }

        sql_statement_text += " WHERE ";

        bool where_is_present = false;

        for (FieldView const& field : field_views)
        {
            if (!field.is_primary_key())
            {
                continue;
            }

            if (!where_is_present)
            {
                where_is_present    = true;
                sql_statement_text += std::string{field.column_name()};
                sql_statement_text += "=?";
            }
        }

        if (!where_is_present)
        {
            throw std::runtime_error(
                "orm::update requires a primary key field"
            );
        }

        sql_statement_text += ";";

        db::Statement statement = database.prepare(sql_statement_text);

        int parameter_index_one_based = 1;

        for (FieldView const& field : field_views)
        {
            if (field.is_primary_key())
            {
                continue;
            }

            field.bind(statement, parameter_index_one_based);
            ++parameter_index_one_based;
        }

        for (FieldView const& field : field_views)
        {
            if (!field.is_primary_key())
            {
                continue;
            }

            field.bind(statement, parameter_index_one_based);
        }

        statement.execute_to_completion();
    }

    template <db_model Model, typename PrimaryKeyValue>
    [[nodiscard]] Model select_by_pk(
        db::Database&          database,
        PrimaryKeyValue const& primary_key_value
    )
    {
        Model      empty_instance{};
        auto const empty_field_views = orm::fields(empty_instance);

        std::string sql_statement_text;
        sql_statement_text += "SELECT ";

        bool first_select_column = true;

        for (FieldView const& field : empty_field_views)
        {
            if (!first_select_column)
            {
                sql_statement_text += ", ";
            }

            first_select_column  = false;
            sql_statement_text  += std::string{field.column_name()};
        }

        sql_statement_text += " FROM ";
        sql_statement_text += std::string{Model::table_name.view()};
        sql_statement_text += " WHERE ";

        bool where_is_present = false;

        for (FieldView const& field : empty_field_views)
        {
            if (!field.is_primary_key())
            {
                continue;
            }

            if (!where_is_present)
            {
                where_is_present    = true;
                sql_statement_text += std::string{field.column_name()};
                sql_statement_text += "=?";
            }
        }

        if (!where_is_present)
        {
            throw std::runtime_error(
                "orm::select_by_pk requires a primary key field"
            );
        }

        sql_statement_text += ";";

        db::Statement statement = database.prepare(sql_statement_text);
        orm::binder<db::Statement, PrimaryKeyValue>::bind(
            statement,
            1,
            primary_key_value
        );

        bool const has_row = statement.step();
        if (!has_row)
        {
            throw std::runtime_error("orm::select_by_pk did not find a row");
        }

        Model      loaded_model{};
        auto const loaded_field_views = orm::fields(loaded_model);

        int column_index_zero_based = 0;

        for (FieldView const& field : loaded_field_views)
        {
            field.read_from(statement, column_index_zero_based);
            ++column_index_zero_based;
        }

        return loaded_model;
    }
}   // namespace orm

#endif   // __ORM__CRUD_HPP__