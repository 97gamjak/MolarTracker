#ifndef __ORM__INCLUDE__ORM__JOIN_HPP__
#define __ORM__INCLUDE__ORM__JOIN_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <string>
#include <vector>

namespace orm
{
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define JOIN_TYPES_LIST(X) \
    X(INNER)               \
    X(LEFT)                \
    X(RIGHT)

    MSTD_ENUM(JoinType, std::uint8_t, JOIN_TYPES_LIST)

    class Join;

    template <typename FromField, typename ToField>
    Join innerJoin();

    template <typename FromField, typename ToField>
    Join join();

    template <typename FromField, typename ToField>
    Join leftJoin();

    template <typename FromField, typename ToField>
    Join rightJoin();

    /**
     * @brief Class representing a SQL JOIN clause
     *
     */
    class Join
    {
       private:
        /// the type of the JOIN
        JoinType _type;
        /// the name of the table to join
        std::string _fromTable;
        /// the name of the field to join on
        std::string _fromField;
        /// the name of the table to join to
        std::string _toTable;
        /// the name of the field to join to
        std::string _toField;

       public:
        explicit Join(JoinType type);

        [[nodiscard]]
        std::string toSQL() const;

        template <typename FromField, typename ToField>
        friend Join innerJoin();

        template <typename FromField, typename ToField>
        friend Join join();

        template <typename FromField, typename ToField>
        friend Join leftJoin();

        template <typename FromField, typename ToField>
        friend Join rightJoin();

       private:
        template <typename FromField, typename ToField>
        friend void _fromFields(Join& join_);
    };

    /**
     * @brief Class representing a collection of SQL JOIN clauses
     *
     */
    class Joins
    {
       private:
        /// the list of JOIN clauses
        std::vector<Join> _joins;

       public:
        [[nodiscard]]
        Joins& add(const Join& join);

        [[nodiscard]]
        std::string toSQL() const;
    };

}   // namespace orm

#ifndef __ORM__INCLUDE__ORM__JOIN_TPP__
#include "join.tpp"
#endif

#endif   // __ORM__INCLUDE__ORM__JOIN_HPP__