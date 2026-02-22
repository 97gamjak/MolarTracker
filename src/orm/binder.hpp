#ifndef __ORM__BINDER_HPP__
#define __ORM__BINDER_HPP__

#include <cstdint>
#include <string>
#include <string_view>

#include "finance/currency.hpp"
#include "orm/concepts.hpp"

namespace orm
{
    template <typename Statement, typename T>
    struct binder;

    /**
     * @brief Binder for 64-bit integer values
     *
     * @tparam Statement
     */
    template <typename Statement>
    struct binder<Statement, std::int64_t>
    {
        /**
         * @brief Bind a 64-bit integer value to the specified parameter index
         *
         * @param statement
         * @param index
         * @param value
         */
        static inline void bind(
            Statement&   statement,
            int          index,
            std::int64_t value
        )
        {
            statement.bindInt64(index, value);
        }

        /**
         * @brief Read a 64-bit integer value from the specified column
         *
         * @param statement
         * @param col
         * @return std::int64_t
         */
        static std::int64_t read(Statement const& statement, int col)
        {
            return statement.columnInt64(col);
        }
    };

    /**
     * @brief Binder for integer values
     *
     * @tparam Statement
     */
    template <typename Statement>
    struct binder<Statement, int>
    {
        /**
         * @brief Bind an integer value to the specified parameter index
         *
         * @param statement
         * @param index
         * @param value
         */
        static void bind(Statement& statement, int index, int value)
        {
            statement.bindInt64(index, static_cast<std::int64_t>(value));
        }

        /**
         * @brief Read an integer value from the specified column
         *
         * @param statement
         * @param col
         * @return int
         */
        static int read(Statement const& statement, int col)
        {
            return static_cast<int>(statement.columnInt64(col));
        }
    };

    /**
     * @brief Binder for boolean values
     *
     * @tparam Statement
     */
    template <typename Statement>
    struct binder<Statement, bool>
    {
        /**
         * @brief Bind a boolean value to the specified parameter index
         *
         * @param statement
         * @param index
         * @param value
         */
        static void bind(Statement& statement, int index, bool value)
        {
            statement.bindInt64(index, value ? 1 : 0);
        }

        /**
         * @brief Read a boolean value from the specified column
         *
         * @param statement
         * @param col
         * @return bool
         */
        static bool read(Statement const& statement, int col)
        {
            return statement.columnInt64(col) != 0;
        }
    };

    /**
     * @brief Binder for double values
     *
     * @tparam Statement
     */
    template <typename Statement>
    struct binder<Statement, double>
    {
        /**
         * @brief Bind a double value to the specified parameter index
         *
         * @param statement
         * @param index
         * @param value
         */
        static void bind(Statement& statement, int index, double value)
        {
            statement.bindDouble(index, value);
        }

        /**
         * @brief Read a double value from the specified column
         *
         * @param statement
         * @param col
         * @return double
         */
        static double read(Statement const& statement, int col)
        {
            return statement.columnDouble(col);
        }
    };

    /**
     * @brief Binder for string values
     *
     * @tparam Statement
     */
    template <typename Statement>
    struct binder<Statement, std::string>
    {
        /**
         * @brief Bind a string value to the specified parameter index
         *
         * @param statement
         * @param index
         * @param value
         */
        static void bind(
            Statement&         statement,
            int                index,
            std::string const& value
        )
        {
            statement.bindText(index, value);
        }

        /**
         * @brief Read a string value from the specified column
         *
         * @param statement
         * @param col
         * @return std::string
         */
        static std::string read(Statement const& statement, int col)
        {
            return statement.columnText(col);
        }
    };

    /**
     * @brief Binder for strong_id types
     *
     * @tparam Statement
     * @tparam T
     */
    template <typename Statement, strong_id T>
    struct binder<Statement, T>
    {
        /**
         * @brief Bind a strong_id value to the specified parameter index
         *
         * @param statement
         * @param index
         * @param value
         */
        static void bind(Statement& statement, int index, T const& value)
        {
            statement.bindInt64(index, value.value());
        }

        /**
         * @brief Read a strong_id value from the specified column
         *
         * @param statement
         * @param col
         * @return T
         */
        static T read(Statement const& statement, int col)
        {
            return T::from(statement.columnInt64(col));
        }
    };

    /**
     * @brief Binder for finance::Currency enum
     *
     * @tparam Statement
     */
    template <typename Statement>
    struct binder<Statement, finance::Currency>
    {
        /// alias for finance::CurrencyMeta
        using CurrencyMeta = finance::CurrencyMeta;

        /**
         * @brief Bind a finance::Currency value to the specified parameter
         * index
         *
         * @param statement
         * @param index
         * @param value
         */
        static void bind(
            Statement&               statement,
            int                      index,
            finance::Currency const& value
        )
        {
            statement.bindText(index, CurrencyMeta::name(value));
        }

        /**
         * @brief Read a finance::Currency value from the specified column
         *
         * @param statement
         * @param col
         * @return finance::Currency
         */
        static finance::Currency read(Statement const& statement, int col)
        {
            return CurrencyMeta::from_string(statement.columnText(col));
        }
    };
}   // namespace orm

#endif   // __ORM__BINDER_HPP__