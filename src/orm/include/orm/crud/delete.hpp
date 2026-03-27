#ifndef __ORM__INCLUDE__ORM__CRUD__DELETE_HPP__
#define __ORM__INCLUDE__ORM__CRUD__DELETE_HPP__

#include "crud_error.hpp"
#include "db/database.hpp"
#include "db/statement.hpp"
#include "logging/log_macros.hpp"
#include "orm/type_traits.hpp"

REGISTER_LOG_CATEGORY("Orm.Crud.Delete");

namespace orm::details
{
    /**
     * @brief Delete a row by primary key value
     *
     * @tparam Model
     * @param database
     * @param model
     */
    template <db_model Model>
    void deleteByPk(db::Database& database, const Model& model)
    {
        const auto numberOfPkFields = getNumberOfPkFields<Model>();

        if (numberOfPkFields != 1)
        {
            throw CrudException(
                "orm::deleteByPk requires a model with exactly one primary "
                "key field"
            );
        }

        std::string sqlText;
        sqlText += "DELETE FROM ";
        sqlText += Model::tableName;
        sqlText += " WHERE ";

        const auto whereClauses = getPkWhereClauses<Model>(model);

        if (whereClauses.empty())
        {
            throw CrudException("orm::deleteByPk requires a primary key field");
        }

        if (whereClauses.size() != numberOfPkFields)
        {
            throw CrudException(
                "orm::deleteByPk requires all primary key fields to be set"
            );
        }

        sqlText += whereClauses.getDBOperations();
        sqlText += ";";

        LOG_DEBUG(
            std::format(
                "Deleting from table '{}' with SQL: {}",
                Model::tableName,
                sqlText
            )
        );

        db::Statement statement = database.prepare(sqlText);
        whereClauses.bind(statement);

        statement.executeToCompletion();
    }
}   // namespace orm::details

#endif   // __ORM__INCLUDE__ORM__CRUD__DELETE_HPP__