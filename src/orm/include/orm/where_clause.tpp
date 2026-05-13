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

    /**
     * @brief Construct a new In Clause< Field>:: In Clause object
     *
     * @tparam Field
     * @param fields
     */
    template <typename Field>
    InClause<Field>::InClause(std::vector<Field> fields)
        : _fields(std::move(fields))
    {
    }

    /**
     * @brief Get the SQL operations for this IN clause, e.g. "field IN (?, ?)"
     *
     * @tparam Field
     * @return std::string
     */
    template <typename Field>
    std::string InClause<Field>::getDBOperations() const
    {
        std::string placeholders;
        for (std::size_t i = 0; i < _fields.size(); ++i)
        {
            if (i > 0)
                placeholders += ", ";
            placeholders += "?";
        }

        return Field::tableName + "." + Field::getFullColumnName() + " IN (" +
               placeholders + ")";
    }

    /**
     * @brief Bind the values for this IN clause to the specified statement,
     * using the specified index for parameter binding
     *
     * @tparam Field
     * @param statement
     * @param index
     */
    template <typename Field>
    void InClause<Field>::bind(db::Statement& statement, BindIndex& index) const
    {
        for (const auto& field : _fields)
        {
            // Rebind _field's value per iteration, then delegate
            field.bind(statement, index);
            ++index;
        }
    }

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__WHERE_CLAUSE_TPP__