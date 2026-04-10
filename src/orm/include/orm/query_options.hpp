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
    /**
     * @brief Class representing options for database queries, such as ordering,
     * limiting results, and filtering with where expressions
     *
     */
    class QueryOptions
    {
       private:
        /// A vector of pairs representing fields to order by and their sort
        /// direction (true for ascending, false for descending)
        std::vector<std::pair<std::string, bool>> _orderFields;

        /// An optional limit on the number of results to return
        std::optional<std::size_t> _limit;

        /// A where expression representing the filtering conditions for the
        /// query
        WhereExpr _whereExpr = makeEmptyWhere();

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

#ifndef __ORM__INCLUDE__ORM__QUERY_OPTIONS_TPP__
#include "query_options.tpp"
#endif

#endif   // __ORM__INCLUDE__ORM__QUERY_OPTIONS_HPP__