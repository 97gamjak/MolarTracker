#ifndef __ORM__INCLUDE__ORM__FIELDS_TPP__
#define __ORM__INCLUDE__ORM__FIELDS_TPP__

#include <mstd/string.hpp>

#include "fields.hpp"
#include "logging/log_macros.hpp"

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
     * @param delimiter The delimiter to use between column names
     * @return std::string
     */
    template <db_model Model>
    std::string getColumnNames(const std::string& delimiter)
    {
        std::vector<std::string> columnNames;

        Model::forEachColumn([&](auto& field)
                             { columnNames.push_back(field.getColumnName()); });

        return mstd::join(columnNames, delimiter);
    }

    /**
     * @brief Get the Pk Where Clauses
     *
     * @tparam Model
     * @param model
     * @return WhereClauses
     */
    template <db_model Model>
    WhereClauses getPkWhereClauses(const Model& model)
    {
        WhereClauses whereClauses;
        model.forEachField(
            [&](const auto& field)
            {
                if (field.isPk)
                {
                    whereClauses.addClause(
                        WhereClause{
                            field,
                            Model::tableName,
                            WhereOperator::Equal
                        }
                    );
                }
            }
        );
        return whereClauses;
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
    template <typename Model>
    Model loadModelFromStatement(db::Statement const& statement)
    {
        LOG_ENTRY;

        Model loadedModel{};

        std::size_t col = 0;

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

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__FIELDS_TPP__