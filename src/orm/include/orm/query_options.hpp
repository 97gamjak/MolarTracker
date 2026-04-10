#ifndef __ORM__INCLUDE__ORM__QUERY_OPTIONS_HPP__
#define __ORM__INCLUDE__ORM__QUERY_OPTIONS_HPP__

#include <cstddef>
#include <mstd/enum.hpp>
#include <string>
#include <vector>

#include "db/statement.hpp"
#include "filter/operators.hpp"
#include "orm/where_expr.hpp"

namespace orm
{
    class QueryOptions
    {
       private:
        std::vector<std::pair<std::string, bool>> _orderFields;
        std::optional<std::size_t>                _limit;
        WhereExpr                                 _whereExpr = makeEmptyWhere();

       public:
        template <typename Field>
        [[nodiscard]] QueryOptions& orderBy(bool ascending);

        [[nodiscard]] QueryOptions& limit(std::size_t limit);

        [[nodiscard]] QueryOptions& where(const WhereExpr& whereExpr);

        template <typename Field>
        [[nodiscard]] QueryOptions& where(
            const Field&     field,
            filter::Operator operator_
        );

        [[nodiscard]] std::string getDBOperations() const;
        [[nodiscard]] std::string getWhereDBOperations() const;

        void bind(db::Statement& statement) const;
    };
}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__QUERY_OPTIONS_HPP__