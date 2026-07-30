#ifndef __ORM__INCLUDE__ORM__WHERE_EXPR_TPP__
#define __ORM__INCLUDE__ORM__WHERE_EXPR_TPP__

#include "orm_exception.hpp"
#include "where_expr.hpp"

namespace orm
{
    /**
     * @brief Create a WHERE expression for a specific field and operator
     *
     * @tparam Field
     * @tparam Value
     * @param field
     * @param operator_
     * @return WhereExpr
     */
    template <typename Field, typename Value>
    WhereExpr makeWhere(const Value& field, filter::Operator operator_)
    {
        return std::make_shared<WhereClause<Field>>(Field(field), operator_);
    }

    /**
     * @brief Create a WHERE expression for a specific field and operator
     *
     * @tparam Field
     * @return WhereExpr
     */
    template <typename Field>
    WhereExpr makeIsNull()
    {
        return std::make_shared<NullClause<Field>>();
    }

    /**
     * @brief Create a WHERE expression for a specific field and operator
     *
     * @tparam Field
     * @tparam Range
     * @param field
     * @param operator_
     * @return WhereExpr
     */
    template <typename Field, std::ranges::input_range Range>
    WhereExpr makeInClause(const Range& values)
    {
        if (std::ranges::empty(values))
            throw ORMError("makeInClause requires at least one value");

        return std::make_shared<InClause<Field>>(
            std::vector<Field>(values.begin(), values.end())
        );
    }
}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__WHERE_EXPR_TPP__
