#ifndef __ORM__INCLUDE__ORM__WHERE_EXPR_HPP__
#define __ORM__INCLUDE__ORM__WHERE_EXPR_HPP__

#include <memory>
#include <string>

#include "filter/expr_node.hpp"
#include "filter/operators.hpp"
#include "orm/where_clause.hpp"

namespace db
{
    class Statement;   // Forward declaration
}   // namespace db

namespace orm
{
    using WhereExpr = filter::Node<std::shared_ptr<IWhereClause>>;

    [[nodiscard]] std::string getDBOperations(const WhereExpr& expr);

    void bind(const WhereExpr& expr, db::Statement& statement);

    void bind(
        const WhereExpr& expr,
        db::Statement&   statement,
        BindIndex&       index
    );

    WhereExpr makeEmptyWhere();

    template <typename Field>
    WhereExpr makeWhere(Field field, filter::Operator operator_);

}   // namespace orm

#ifndef __ORM__INCLUDE__ORM__WHERE_EXPR_TPP__
#include "where_expr.tpp"
#endif

#endif   // __ORM__INCLUDE__ORM__WHERE_EXPR_HPP__