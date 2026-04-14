#include "orm/where_expr.hpp"

#include <memory>

#include "db/statement.hpp"
#include "orm/index.hpp"
#include "orm/where_clause.hpp"

namespace orm
{
    using IWhereClausePtr      = std::shared_ptr<IWhereClause>;
    using AndIWhereClausePtr   = filter::AndNode<IWhereClausePtr>;
    using OrIWhereClausePtr    = filter::OrNode<IWhereClausePtr>;
    using NotIWhereClausePtr   = filter::NotNode<IWhereClausePtr>;
    using EmptyIWhereClausePtr = filter::EmptyNode<IWhereClausePtr>;

    /**
     * @brief Visitor for generating database operations from filter
     * expressions.
     *
     */
    struct DBOperationVisitor
    {
        /**
         * @brief Generate database operations for a filter expression.
         *
         * @param expr The filter expression.
         * @return The generated database operations.
         */
        std::string operator()(const IWhereClausePtr& expr) const
        {
            return expr->getDBOperations();
        }

        /**
         * @brief Generate database operations for a conjunction (AND) filter
         * expression.
         *
         * @param expr The conjunction (AND) filter expression.
         * @return The generated database operations.
         */
        std::string operator()(const AndIWhereClausePtr& expr) const
        {
            return "(" + getDBOperations(*expr.left) + ") AND (" +
                   getDBOperations(*expr.right) + ")";
        }

        /**
         * @brief Generate database operations for a disjunction (OR) filter
         * expression.
         *
         * @param expr The disjunction (OR) filter expression.
         * @return The generated database operations.
         */
        std::string operator()(const OrIWhereClausePtr& expr) const
        {
            return "(" + getDBOperations(*expr.left) + ") OR (" +
                   getDBOperations(*expr.right) + ")";
        }

        /**
         * @brief Generate database operations for a negation (NOT) filter
         * expression.
         *
         * @param expr The negation (NOT) filter expression.
         * @return The generated database operations.
         */
        std::string operator()(const NotIWhereClausePtr& expr) const
        {
            return "NOT (" + getDBOperations(*expr.value) + ")";
        }

        /**
         * @brief Generate database operations for an empty filter expression.
         *
         * @return The generated database operations.
         */
        std::string operator()(const EmptyIWhereClausePtr& /*expr*/) const
        {
            return "";
        }
    };

    /**
     * @brief Visitor for binding filter expressions to database statements.
     *
     */
    struct BindVisitor
    {
        /// The database statement to bind to.
        db::Statement& statement;
        /// The bind index to use.
        BindIndex& index;

        /**
         * @brief Bind a filter expression to a database statement.
         *
         * @param expr The filter expression.
         */
        void operator()(const IWhereClausePtr& expr)
        {
            expr->bind(statement, index);
        }

        /**
         * @brief Bind a conjunction (AND) filter expression to a database
         * statement.
         *
         * @param expr The conjunction (AND) filter expression.
         */
        void operator()(const AndIWhereClausePtr& expr)
        {
            bind(*expr.left, statement, index);
            bind(*expr.right, statement, index);
        }

        /**
         * @brief Bind a disjunction (OR) filter expression to a database
         * statement.
         *
         * @param expr The disjunction (OR) filter expression.
         */
        void operator()(const OrIWhereClausePtr& expr)
        {
            bind(*expr.left, statement, index);
            bind(*expr.right, statement, index);
        }

        /**
         * @brief Bind a negation (NOT) filter expression to a database
         * statement.
         *
         * @param expr The negation (NOT) filter expression.
         */
        void operator()(const NotIWhereClausePtr& expr)
        {
            bind(*expr.value, statement, index);
        }

        /**
         * @brief Bind an empty filter expression to a database statement.
         *
         */
        void operator()(const EmptyIWhereClausePtr& /*expr*/) const {}

        /**
         * @brief Construct a BindVisitor.
         *
         * @param _statement The database statement to bind to.
         * @param _index The bind index to use.
         */
        BindVisitor(db::Statement& _statement, BindIndex& _index)
            : statement(_statement), index(_index)
        {
        }

        ~BindVisitor() = default;
        // delete constructor and assignment operator
        BindVisitor(const BindVisitor&)            = delete;
        BindVisitor& operator=(const BindVisitor&) = delete;
        BindVisitor(BindVisitor&&)                 = delete;
        BindVisitor& operator=(BindVisitor&&)      = delete;
    };

    /**
     * @brief Generate SQL operations from a WHERE expression.
     *
     * @param expr The WHERE expression.
     * @return The generated SQL operations.
     */
    [[nodiscard]] std::string getDBOperations(const WhereExpr& expr)
    {
        const auto operations = std::visit(DBOperationVisitor{}, expr);
        return operations.empty() ? "" : "WHERE " + operations;
    }

    /**
     * @brief Bind a WHERE expression to a database statement.
     *
     * @param expr The WHERE expression.
     * @param statement The database statement.
     */
    void bind(const WhereExpr& expr, db::Statement& statement)
    {
        auto index = bindIndex(0);
        std::visit(BindVisitor{statement, index}, expr);
    }

    /**
     * @brief Bind a WHERE expression to a database statement.
     *
     * @param expr The WHERE expression.
     * @param statement The database statement.
     * @param index The bind index.
     */
    void bind(const WhereExpr& expr, db::Statement& statement, BindIndex& index)
    {
        std::visit(BindVisitor{statement, index}, expr);
    }

    /**
     * @brief Create an empty WHERE expression.
     *
     * @return The empty WHERE expression.
     */
    WhereExpr makeEmptyWhere()
    {
        return filter::EmptyNode<std::shared_ptr<IWhereClause>>{};
    }

}   // namespace orm
