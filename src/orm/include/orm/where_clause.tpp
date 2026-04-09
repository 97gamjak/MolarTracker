#ifndef __ORM__INCLUDE__ORM__WHERE_CLAUSE_TPP__
#define __ORM__INCLUDE__ORM__WHERE_CLAUSE_TPP__

#include "filter/operators.hpp"
#include "index.hpp"
#include "orm_exception.hpp"
#include "where_clause.hpp"

namespace orm
{
    /**
     * @brief Construct a new Where Clause< Field>:: Where Clause object
     *
     * @tparam Field
     * @param field
     * @param operator_
     */
    template <typename Field>
    WhereClause<Field>::WhereClause(Field field, filter::Operator operator_)
        : _field(std::move(field)), _operator(operator_)
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

        return _field.getFullColumnName() + " " + operatorStr;
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