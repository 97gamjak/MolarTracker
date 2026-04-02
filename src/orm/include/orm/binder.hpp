#ifndef __ORM__INCLUDE__ORM__BINDER_HPP__
#define __ORM__INCLUDE__ORM__BINDER_HPP__

#include <cstdint>
#include <mstd/type_traits.hpp>
#include <string>

#include "concepts.hpp"
#include "index.hpp"
#include "orm_exception.hpp"
#include "utils/timestamp.hpp"

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
        static void bind(
            Statement&   statement,
            BindIndex    index,
            std::int64_t value
        )
        {
            statement.bindInt64(index.value(), value);
        }

        /**
         * @brief Read a 64-bit integer value from the specified column
         *
         * @param statement
         * @param col
         * @return std::int64_t
         */
        static std::int64_t read(Statement const& statement, ColumnIndex col)
        {
            return statement.columnInt64(col.value());
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
        static void bind(Statement& statement, BindIndex index, int value)
        {
            statement.bindInt64(
                index.value(),
                static_cast<std::int64_t>(value)
            );
        }

        /**
         * @brief Read an integer value from the specified column
         *
         * @param statement
         * @param col
         * @return int
         */
        static int read(Statement const& statement, ColumnIndex col)
        {
            return static_cast<int>(statement.columnInt64(col.value()));
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
        static void bind(Statement& statement, BindIndex index, bool value)
        {
            statement.bindInt64(index.value(), value ? 1 : 0);
        }

        /**
         * @brief Read a boolean value from the specified column
         *
         * @param statement
         * @param col
         * @return bool
         */
        static bool read(Statement const& statement, ColumnIndex col)
        {
            return statement.columnInt64(col.value()) != 0;
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
        static void bind(Statement& statement, BindIndex index, double value)
        {
            statement.bindDouble(index.value(), value);
        }

        /**
         * @brief Read a double value from the specified column
         *
         * @param statement
         * @param col
         * @return double
         */
        static double read(Statement const& statement, ColumnIndex col)
        {
            return statement.columnDouble(col.value());
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
            BindIndex          index,
            std::string const& value
        )
        {
            statement.bindText(index.value(), value);
        }

        /**
         * @brief Read a string value from the specified column
         *
         * @param statement
         * @param col
         * @return std::string
         */
        static std::string read(Statement const& statement, ColumnIndex col)
        {
            return statement.columnText(col.value());
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
        static void bind(Statement& statement, BindIndex index, T const& value)
        {
            statement.bindInt64(index.value(), value.value());
        }

        /**
         * @brief Read a strong_id value from the specified column
         *
         * @param statement
         * @param col
         * @return T
         */
        static T read(Statement const& statement, ColumnIndex col)
        {
            return T::from(statement.columnInt64(col.value()));
        }
    };

    /**
     * @brief Binder for finance::Currency enum
     *
     * @tparam Statement
     */
    template <typename Statement, typename T>
    requires mstd::has_enum_meta<T>
    struct binder<Statement, T>
    {
        /// alias for enum_meta_t<T>
        using EnumMeta = mstd::enum_meta_t<T>;

        /**
         * @brief Bind a T value to the specified parameter index
         *
         * @param statement
         * @param index
         * @param value
         */
        static void bind(Statement& statement, BindIndex index, T const& value)
        {
            const auto name = EnumMeta::name(value);
            statement.bindText(index.value(), std::string{name});
        }

        /**
         * @brief Read a finance::Currency value from the specified column
         *
         * @param statement
         * @param col
         * @return T
         */
        static T read(Statement const& statement, ColumnIndex col)
        {
            const auto value =
                EnumMeta::from_string(statement.columnText(col.value()));

            if (!value.has_value())
            {
                throw ORMError(
                    "Invalid currency value in database: " +
                    statement.columnText(col.value())
                );
            }

            return value.value();
        }
    };

    template <typename Statement>
    struct binder<Statement, Timestamp>
    {
        /**
         * @brief Bind a Timestamp value to the specified parameter index
         *
         * @param statement
         * @param index
         * @param value
         */
        static void bind(
            Statement&       statement,
            BindIndex        index,
            Timestamp const& value
        )
        {
            statement.bindInt64(index.value(), value.toInt64());
        }

        /**
         * @brief Read a Timestamp value from the specified column
         *
         * @param statement
         * @param col
         * @return Timestamp
         */
        static Timestamp read(Statement const& statement, ColumnIndex col)
        {
            return Timestamp::fromInt64(statement.columnInt64(col.value()));
        }
    };
}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__BINDER_HPP__