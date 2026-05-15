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
     * @return Query&
     */
    template <typename Field>
    Query& Query::orderBy(bool ascending)
    {
        _orderFields.emplace_back(std::string(Field::name), ascending);
        return *this;
    }

    /**
     * @brief set a WHERE expression for the query
     *
     * @tparam Field
     * @tparam Value
     * @param field
     * @param operator_
     * @return Query&
     */
    template <typename Field, typename Value>
    Query& Query::where(const Value& field, filter::Operator operator_)
    {
        _whereExpr &= makeWhere<Field>(field, operator_);
        return *this;
    }

    /**
     * @brief Add an IN clause for the specified field
     *
     * @tparam Field
     * @tparam Range
     * @param values
     * @return Query&
     */
    template <typename Field, std::ranges::input_range Range>
    Query& Query::in(const Range& values)
    {
        _whereExpr &= makeInClause<Field>(values);
        return *this;
    }

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__QUERY_OPTIONS_TPP__
