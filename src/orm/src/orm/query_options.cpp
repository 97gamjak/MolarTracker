#include "orm/query_options.hpp"

#include <mstd/string.hpp>

#include "orm/where_expr.hpp"

namespace orm
{
    QueryOptions& QueryOptions::limit(std::size_t limit)
    {
        _limit = limit;
        return *this;
    }

    QueryOptions& QueryOptions::where(const WhereExpr& whereExpr)
    {
        _whereExpr &= whereExpr;
        return *this;
    }

    std::string QueryOptions::getWhereDBOperations() const
    {
        return orm::getDBOperations(_whereExpr);
    }

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

    void QueryOptions::bind(db::Statement& statement) const
    {
        orm::bind(_whereExpr, statement);
    }

}   // namespace orm
