#ifndef __ORM__INCLUDE__ORM__QUERY_OPTIONS_TPP__
#define __ORM__INCLUDE__ORM__QUERY_OPTIONS_TPP__

#include "orm/where_expr.hpp"
#include "query_options.hpp"

namespace orm
{
    /**
     * @brief Add an order by clause for the specified field
     *
     * @tparam Field
     * @param ascending
     * @return QueryOptions&
     */
    template <typename Field>
    QueryOptions& QueryOptions::orderBy(bool ascending)
    {
        _orderFields.push_back({Field::name, ascending});
        return *this;
    }

    /**
     * @brief set a WHERE expression for the query
     *
     * @param field
     * @param operator_
     * @return QueryOptions&
     */
    template <typename Field>
    QueryOptions& QueryOptions::where(
        const Field&     field,
        filter::Operator operator_
    )
    {
        _whereExpr &= makeWhere(field, operator_);
        return *this;
    }

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__QUERY_OPTIONS_TPP__
