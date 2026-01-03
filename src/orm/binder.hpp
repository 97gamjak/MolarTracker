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
            statement.bind_int64(index, value);
        }

        static std::int64_t read(Statement const& statement, int col)
        {
            return statement.column_int64(col);
        }
    };

    template <typename Statement>
    struct binder<Statement, int>
    {
        static void bind(Statement& statement, int index, int value)
        {
            statement.bind_int64(index, static_cast<std::int64_t>(value));
        }

        static int read(Statement const& statement, int col)
        {
            return static_cast<int>(statement.column_int64(col));
        }
    };

    template <typename Statement>
    struct binder<Statement, bool>
    {
        static void bind(Statement& statement, int index, bool value)
        {
            statement.bind_int64(index, value ? 1 : 0);
        }

        static bool read(Statement const& statement, int col)
        {
            return statement.column_int64(col) != 0;
        }
    };

    template <typename Statement>
    struct binder<Statement, double>
    {
        static void bind(Statement& statement, int index, double value)
        {
            statement.bind_double(index, value);
        }

        static double read(Statement const& statement, int col)
        {
            return statement.column_double(col);
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
            statement.bind_text(index, value);
        }

        static std::string read(Statement const& statement, int col)
        {
            return statement.column_text(col);
        }
    };

    template <typename Statement, strong_id T>
    struct binder<Statement, T>
    {
        static void bind(Statement& statement, int index, T const& value)
        {
            statement.bind_int64(index, value.value());
        }

        static T read(Statement const& statement, int col)
        {
            return T::from(statement.column_int64(col));
        }
    };
}   // namespace orm

#endif   // __ORM__BINDER_HPP__