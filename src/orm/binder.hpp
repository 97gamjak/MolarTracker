#ifndef __ORM__BINDER_HPP__
#define __ORM__BINDER_HPP__

#include <cstdint>
#include <string>
#include <string_view>

#include "orm/concepts.hpp"

namespace orm
{
    template <typename Statement, typename T>
    struct binder;

    template <typename Statement>
    struct binder<Statement, std::int64_t>
    {
        static inline void bind(
            Statement&   statement,
            int          index,
            std::int64_t value
        )
        {
            statement.bindInt64(index, value);
        }

        static std::int64_t read(Statement const& statement, int col)
        {
            return statement.columnInt64(col);
        }
    };

    template <typename Statement>
    struct binder<Statement, int>
    {
        static void bind(Statement& statement, int index, int value)
        {
            statement.bindInt64(index, static_cast<std::int64_t>(value));
        }

        static int read(Statement const& statement, int col)
        {
            return static_cast<int>(statement.columnInt64(col));
        }
    };

    template <typename Statement>
    struct binder<Statement, bool>
    {
        static void bind(Statement& statement, int index, bool value)
        {
            statement.bindInt64(index, value ? 1 : 0);
        }

        static bool read(Statement const& statement, int col)
        {
            return statement.columnInt64(col) != 0;
        }
    };

    template <typename Statement>
    struct binder<Statement, double>
    {
        static void bind(Statement& statement, int index, double value)
        {
            statement.bindDouble(index, value);
        }

        static double read(Statement const& statement, int col)
        {
            return statement.columnDouble(col);
        }
    };

    template <typename Statement>
    struct binder<Statement, std::string>
    {
        static void bind(
            Statement&         statement,
            int                index,
            std::string const& value
        )
        {
            statement.bindText(index, value);
        }

        static std::string read(Statement const& statement, int col)
        {
            return statement.columnText(col);
        }
    };

    template <typename Statement, strong_id T>
    struct binder<Statement, T>
    {
        static void bind(Statement& statement, int index, T const& value)
        {
            statement.bindInt64(index, value.value());
        }

        static T read(Statement const& statement, int col)
        {
            return T::from(statement.columnInt64(col));
        }
    };
}   // namespace orm

#endif   // __ORM__BINDER_HPP__