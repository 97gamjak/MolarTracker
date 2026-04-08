#ifndef __ORM__INCLUDE__ORM__WHERE_CLAUSE_TPP__
#define __ORM__INCLUDE__ORM__WHERE_CLAUSE_TPP__

#include "filter/operators.hpp"
#include "orm/index.hpp"
#include "orm/orm_exception.hpp"
#include "where_clause.hpp"

namespace orm
{
    /**
     * @brief Construct a new Where Clause< Field>:: Where Clause object
     *
     * @tparam Field
     * @param field
     * @param tableName
     * @param operator_
     */
    template <typename Field>
    WhereClause<Field>::WhereClause(
        Field            field,
        std::string      tableName,
        filter::Operator operator_
    )
        : _field(std::move(field)),
          _tableName(std::move(tableName)),
          _operator(operator_)
    {
    }

    /**
     * @brief Get the SQL operations for this where clause, e.g. "field = ?"
     *
     * @tparam Field
     * @return std::string
     */
    template <typename Field>
    std::string WhereClause<Field>::getDBOperations() const
    {
        const auto operatorStr = whereOperatorStr(_operator);

        if (operatorStr.empty())
            throw ORMError("Invalid WhereOperator value");

        if (!_tableName.empty())
            return _tableName + "." + _field.getColumnName() + " " +
                   operatorStr;

        return _field.getColumnName() + " " + operatorStr;
    }

    /**
     * @brief Bind the values for this where clause to the specified statement,
     * using the specified index for parameter binding
     *
     * @tparam Field
     * @param statement
     * @param index
     */
    template <typename Field>
    void WhereClause<Field>::bind(
        db::Statement& statement,
        BindIndex&     index
    ) const
    {
        _field.bind(statement, index);
        ++index;
    }

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__WHERE_CLAUSE_TPP__