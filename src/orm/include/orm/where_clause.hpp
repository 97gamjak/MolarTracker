#ifndef __ORM__INCLUDE__ORM__WHERE_CLAUSE_HPP__
#define __ORM__INCLUDE__ORM__WHERE_CLAUSE_HPP__

#include <mstd/enum.hpp>

#include "filter/operators.hpp"
#include "orm/index.hpp"

namespace db
{
    class Statement;   // Forward declaration
}   // namespace db

namespace orm
{

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
        virtual void bind(db::Statement& statement, BindIndex& index) const = 0;
    };

    [[nodiscard]] std::string whereOperatorStr(filter::Operator operator_);

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

        /// the operator to use for the comparison
        filter::Operator _operator;

       public:
        explicit WhereClause(Field field, filter::Operator operator_);

        [[nodiscard]] std::string getDBOperations() const override;

        void bind(db::Statement& statement, BindIndex& index) const override;
    };

}   // namespace orm

#ifndef __ORM__INCLUDE__ORM__WHERE_CLAUSE_TPP__
#include "where_clause.tpp"   // IWYU pragma: keep
#endif

#endif   // __ORM__INCLUDE__ORM__WHERE_CLAUSE_HPP__