#ifndef __ORM__CRUD__DELETE_HPP__
#define __ORM__CRUD__DELETE_HPP__

#include "crud_detail.hpp"
#include "crud_error.hpp"
#include "db/database.hpp"
#include "orm/binder.hpp"
#include "orm/constraints.hpp"
#include "orm/fields.hpp"
#include "orm/type_traits.hpp"

namespace orm
{
    /**
     * @brief Delete a row by primary key value
     *
     * @tparam Model
     * @tparam PrimaryKeyValue
     * @param database
     * @param pkValue
     */
    template <db_model Model, typename PrimaryKeyValue>
    void deleteByPk(db::Database& database, PrimaryKeyValue const& pkValue)
    {
        const auto numberOfPkFields = getNumberOfPkFields<Model>();

        if (numberOfPkFields != 1)
        {
            throw CrudException(
                "orm::deleteByPk requires a model with exactly one primary "
                "key field"
            );
        }

        auto const emptyFieldViews = orm::fields<Model>();

        std::string sqlText;
        sqlText += "DELETE FROM ";
        sqlText += Model::table_name;
        sqlText += " WHERE ";

        bool whereIsPresent = false;

        const auto whereClauses = getColumnNames(
            emptyFieldViews,
            ORMConstraint::PrimaryKey,
            ORMConstraintMode::Only,
            "=?"
        );

        if (!whereIsPresent)
        {
            throw CrudException("orm::deleteByPk requires a primary key field");
        }

        sqlText                 += ";";
        db::Statement statement  = database.prepare(sqlText);
        orm::binder<db::Statement, PrimaryKeyValue>::bind(
            statement,
            1,
            pkValue
        );

        statement.executeToCompletion();
    }
}   // namespace orm

#endif   // __ORM__CRUD__DELETE_HPP__