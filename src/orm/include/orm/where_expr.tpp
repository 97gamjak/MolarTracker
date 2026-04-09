#ifndef __ORM__INCLUDE__ORM__WHERE_EXPR_TPP__
#define __ORM__INCLUDE__ORM__WHERE_EXPR_TPP__

#include "where_expr.hpp"

namespace orm
{
    template <typename Field>
    WhereExpr makeWhere(Field field, filter::Operator operator_)
    {
        return std::make_shared<WhereClause<Field>>(
            std::move(field),
            operator_
        );
    }
}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__WHERE_EXPR_TPP__
