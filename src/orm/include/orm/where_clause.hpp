#ifndef __ORM__INCLUDE__ORM__WHERE_CLAUSE_HPP__
#define __ORM__INCLUDE__ORM__WHERE_CLAUSE_HPP__

#include <cstdint>
#include <memory>
#include <mstd/enum.hpp>
#include <vector>

#include "orm/index.hpp"

namespace db
{
    class Statement;   // Forward declaration
}   // namespace db

namespace orm
{

    // NOLINTBEGIN(cppcoreguidelines-macro-usage)
#define WHERE_OPERATOR_LIST(X) \
    X(Equal)                   \
    X(NotEqual)                \
    X(Less)                    \
    X(LessOrEqual)             \
    X(Greater)                 \
    X(GreaterOrEqual)

    MSTD_ENUM(WhereOperator, std::uint8_t, WHERE_OPERATOR_LIST);
    // NOLINTEND(cppcoreguidelines-macro-usage)

    /**
     * @brief Interface for a where clause, which can be used to generate SQL
     * operations and bind values to a statement
     *
     */
    class IWhereClause
    {
       public:
        virtual ~IWhereClause() = default;

        /**
         * @brief Get the SQL operations for this where clause, e.g. "field = ?"
         *
         * @return std::string
         */
        [[nodiscard]] virtual std::string getDBOperations() const = 0;

        /**
         * @brief Bind the values for this where clause to the specified
         * statement, using the specified index for parameter binding
         *
         * @param statement
         * @param index
         */
        virtual void bind(db::Statement& statement, BindIndex index) const = 0;
    };

    /**
     * @brief A where clause for a specific field and operator
     *
     * @tparam Field
     */
    template <typename Field>
    class WhereClause : public IWhereClause
    {
       private:
        /// the field to compare with and containing the value to
        /// compare against
        Field _field;

        /// table name for joins
        std::string _tableName;

        /// the operator to use for the comparison
        WhereOperator _operator;

       public:
        explicit WhereClause(
            Field         field,
            std::string   tableName,
            WhereOperator operator_
        );

        [[nodiscard]] std::string getDBOperations() const override;

        void bind(db::Statement& statement, BindIndex index) const override;
    };

    /**
     * @brief A where clause that compares a field to a unique value, e.g. for
     * primary key lookups
     *
     * @tparam Field
     */
    template <typename Field>
    class UniqueClause : public WhereClause<Field>
    {
       public:
        explicit UniqueClause(Field field, std::string tableName);
    };

    template <typename Field>
    using EqualClause = UniqueClause<Field>;

    /**
     * @brief A collection of where clauses that can be combined with AND
     *
     */
    class WhereClauses
    {
       private:
        /// the individual where clauses that make up this collection
        std::vector<std::unique_ptr<IWhereClause>> _clauses;

       public:
        WhereClauses() = default;

        template <typename... Clauses>
        explicit WhereClauses(Clauses... clauses);

        template <typename Field>
        void addClause(Field field, WhereOperator operator_);

        template <typename Field>
        void addClause(WhereClause<Field> clause);

        [[nodiscard]] std::string getDBOperations() const;

        void bind(db::Statement& statement) const;

        [[nodiscard]] bool        empty() const;
        [[nodiscard]] std::size_t size() const;
    };

}   // namespace orm

#ifndef __ORM__INCLUDE__ORM__WHERE_CLAUSE_TPP__
#include "where_clause.tpp"   // IWYU pragma: keep
#endif

#endif   // __ORM__INCLUDE__ORM__WHERE_CLAUSE_HPP__