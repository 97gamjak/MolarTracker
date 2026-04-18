#ifndef __ORM__INCLUDE__ORM__FIELDS_TPP__
#define __ORM__INCLUDE__ORM__FIELDS_TPP__

#include <mstd/string.hpp>

#include "fields.hpp"
#include "logging/log_macros.hpp"
#include "orm/where_expr.hpp"

REGISTER_LOG_CATEGORY("Orm.Fields");

namespace orm
{
    /**
     * @brief Get the DDL strings for all fields in the specified model, e.g.
     * "id INTEGER PRIMARY KEY AUTOINCREMENT"
     *
     * @tparam Model
     * @return std::vector<std::string>
     */
    template <db_model Model>
    std::vector<std::string> getDDl()
    {
        std::vector<std::string> ddlStrings;

        Model::forEachColumn([&](auto& field)
                             { ddlStrings.push_back(field.ddl()); });

        return ddlStrings;
    }

    /**
     * @brief Get the foreign key constraint strings for all fields in the
     * specified model that are foreign keys, e.g.
     * "FOREIGN KEY (profile_id) REFERENCES profile(id) ON DELETE RESTRICT"
     *
     * @tparam Model
     * @return std::vector<std::string>
     */
    template <db_model Model>
    std::vector<std::string> getFKConstraints()
    {
        std::vector<std::string> fkConstraints;

        Model::forEachColumn(
            [&](auto& field)
            {
                if constexpr (field.isFk)
                    fkConstraints.push_back(field.getFkConstraints());
            }
        );

        return fkConstraints;
    }

    /**
     * @brief Get the Column Names
     *
     * @tparam Model
     * @return std::vector<std::string>
     */
    template <db_model Model>
    std::vector<std::string> getColumnNames()
    {
        std::vector<std::string> columnNames;

        Model::forEachColumn([&](auto& field)
                             { columnNames.push_back(field.getColumnName()); });

        return columnNames;
    }

    /**
     * @brief Get the Column Names
     *
     * @tparam Model
     * @return std::vector<std::string>
     */
    template <db_model Model>
    std::vector<std::string> getFullColumnNames()
    {
        std::vector<std::string> columnNames;

        Model::forEachColumn(
            [&](auto& field)
            { columnNames.push_back(field.getFullColumnName()); }
        );

        return columnNames;
    }

    /**
     * @brief Get the Column Names
     *
     * @tparam Model
     * @param delimiter The delimiter to use between column names
     * @return std::string
     */
    template <db_model Model>
    std::string getColumnNames(const std::string& delimiter)
    {
        std::vector<std::string> columnNames;

        Model::forEachColumn([&](auto& field)
                             { columnNames.push_back(std::string(field.name)); }
        );

        return mstd::join(columnNames, delimiter);
    }

    /**
     * @brief Get the Pk Where
     *
     * @tparam Model
     * @param model
     * @return WhereExpr
     */
    template <db_model Model>
    WhereExpr getPkWhere(const Model& model)
    {
        auto where = makeEmptyWhere();
        model.forEachField(
            [&](const auto& field)
            {
                if (field.isPk)
                    where &= makeWhere(field, filter::Operator::Equal);
            }
        );
        return where;
    }

    /**
     * @brief Get the Number Of Pk Fields
     *
     * @tparam Model
     * @return std::size_t
     */
    template <db_model Model>
    std::size_t getNumberOfPkFields()
    {
        std::size_t count = 0;

        Model::forEachColumn(
            [&](const auto& field)
            {
                if (field.isPk)
                    ++count;
            }
        );

        return count;
    }

    /**
     * @brief Get the Number Of Auto Increment Pk Fields
     *
     * @tparam Model
     * @return std::size_t
     */
    template <db_model Model>
    std::size_t getNumberOfAutoIncrementPkFields()
    {
        std::size_t count = 0;

        Model::forEachColumn(
            [&](const auto& field)
            {
                if (field.isAutoIncrementPk)
                    ++count;
            }
        );

        return count;
    }

    /**
     * @brief Get the Number Of Fields
     *
     * @tparam Model
     * @return std::size_t
     */
    template <db_model Model>
    std::size_t getNumberOfFields()
    {
        std::size_t count = 0;

        Model::forEachColumn([&](const auto& /*field*/) { ++count; });

        return count;
    }

    /**
     * @brief Load a model from a database statement, reading the field values
     * from the statement's columns in order
     *
     * @tparam Model
     * @param statement
     * @return Model
     */
    template <db_model Model>
    Model loadModelFromStatement(
        db::Statement const& statement,
        std::size_t          offset
    )
    {
        LOG_ENTRY;

        Model loadedModel{};

        std::size_t col = offset;

        loadedModel.forEachField(
            [&](auto& field)
            {
                field.readFrom(statement, columnIndex(col));
                ++col;
            }
        );

        LOG_DEBUG(std::format("Loaded model {}", loadedModel.toString()));

        return loadedModel;
    }

    template <db_model Model>
    Model loadModelFromStatement(db::Statement const& statement)
    {
        return loadModelFromStatement<Model>(statement, 0);
    }

    template <typename Model>
    auto loadAnyFromStatement(
        const db::Statement& statement,
        std::size_t          offset
    )
    {
        if constexpr (optional_model<Model>)
        {
            return loadOptionalFromStatement<typename Model::value_type>(
                statement,
                offset
            );
        }

        return loadModelFromStatement<Model>(statement, offset);
    }

    template <db_model... Models>
    std::tuple<Models...> loadTupleFromStatement(const db::Statement& statement)
    {
        std::size_t offset = 0;
        return std::tuple<Models...>{
            [&]()
            {
                auto model  = loadAnyFromStatement<Models>(statement, offset);
                offset     += getNumberOfFields<Models>();
                return model;
            }()...
        };
    }

    template <db_model... Models>
    std::string getSelection()
    {
        using BaseModel = std::tuple_element_t<0, std::tuple<Models...>>;

        std::vector<std::string> tables;
        (tables.push_back(std::string(Models::tableName) + ".*"), ...);

        std::string sql  = "SELECT ";
        sql             += mstd::join(tables, ", ");
        sql             += " FROM ";
        sql             += BaseModel::tableName;

        return sql;
    }

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__FIELDS_TPP__