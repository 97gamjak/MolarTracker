#ifndef __ORM__WHERE_CLAUSE_TPP__
#define __ORM__WHERE_CLAUSE_TPP__

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
    WhereClause<Field>::WhereClause(Field field, WhereOperator operator_)
        : _field(std::move(field)), _operator(operator_)
    {
    }

    /**
     * @brief Construct a new Unique Clause< Field>:: Unique Clause object
     *
     * @tparam Field
     * @param field
     */
    template <typename Field>
    UniqueClause<Field>::UniqueClause(Field field)
        : WhereClause<Field>(std::move(field), WhereOperator::Equal)
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
        std::string operatorStr;

        switch (_operator)
        {
            case WhereOperator::Equal:
                operatorStr = " = ?";
                break;
            case WhereOperator::NotEqual:
                operatorStr = " != ?";
                break;
            case WhereOperator::Less:
                operatorStr = " < ?";
                break;
            case WhereOperator::LessOrEqual:
                operatorStr = " <= ?";
                break;
            case WhereOperator::Greater:
                operatorStr = " > ?";
                break;
            case WhereOperator::GreaterOrEqual:
                operatorStr = " >= ?";
                break;
        }

        if (operatorStr.empty())
            throw ORMError("Invalid WhereOperator value");

        return _field.getColumnName() + operatorStr;
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
        BindIndex      index
    ) const
    {
        _field.bind(statement, index);
    }

    /**
     * @brief Add a new where clause to the collection of where clauses
     *
     * @tparam Field
     * @param field
     * @param operator_
     */
    template <typename Field>
    void WhereClauses::addClause(Field field, WhereOperator operator_)
    {
        _clauses.push_back(
            std::make_unique<WhereClause<Field>>(field, operator_)
        );
    }

    /**
     * @brief Add a new where clause to the collection of where clauses
     *
     * @tparam Field
     * @param clause
     */
    template <typename Field>
    void WhereClauses::addClause(WhereClause<Field> clause)
    {
        _clauses.push_back(
            std::make_unique<WhereClause<Field>>(std::move(clause))
        );
    }

}   // namespace orm

#endif   // __ORM__WHERE_CLAUSE_TPP__