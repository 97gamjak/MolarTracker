#include "orm/query_options.hpp"

#include <mstd/string.hpp>

#include "orm/where_expr.hpp"

namespace orm
{
    /**
     * @brief set a limit on the number of results to return
     *
     * @param limit
     * @return QueryOptions&
     */
    QueryOptions& QueryOptions::limit(std::size_t limit)
    {
        _limit = limit;
        return *this;
    }

    /**
     * @brief set a WHERE expression for the query
     *
     * @param whereExpr
     * @return QueryOptions&
     */
    QueryOptions& QueryOptions::where(const WhereExpr& whereExpr)
    {
        _whereExpr &= whereExpr;
        return *this;
    }

    /**
     * @brief Get the DB Operations for the where expression
     *
     * @return std::string
     */
    std::string QueryOptions::getWhereDBOperations() const
    {
        return orm::getDBOperations(_whereExpr);
    }

    /**
     * @brief Get the DB Operations for the query options, including ordering
     * and limiting
     *
     * @return std::string
     */
    std::string QueryOptions::getDBOperations() const
    {
        auto sql = getWhereDBOperations();

        if (_orderFields.empty())
            return sql;

        std::vector<std::string> operations;
        operations.reserve(_orderFields.size());

        for (const auto& [field, ascending] : _orderFields)
            operations.push_back(field + (ascending ? " ASC" : " DESC"));

        sql += "ORDER BY " + mstd::join(operations, ", ");

        if (_limit.has_value())
            sql += " LIMIT " + std::to_string(_limit.value());

        return sql;
    }

    /**
     * @brief Bind the values for the where expression to the statement
     *
     * @param statement
     */
    void QueryOptions::bind(db::Statement& statement) const
    {
        orm::bind(_whereExpr, statement);
    }

}   // namespace orm
