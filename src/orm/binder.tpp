#ifndef __ORM__BINDER_TPP__
#define __ORM__BINDER_TPP__

#include <cstdint>
#include <string>
#include <string_view>

#include "binder.hpp"
#include "concepts.hpp"
#include "sqlite_type.hpp"

namespace orm
{
    template <typename Statement>
    static void binder<Statement, std::int64_t>::bind(
        Statement&   statement,
        int          index,
        std::int64_t value
    )
    {
        statement.bind_int64(index, value);
    }

    template <typename Statement>
    static std::int64_t binder<Statement, std::int64_t>::read(
        Statement const& statement,
        int              col
    )
    {
        return statement.column_int64(col);
    }

    template <typename Statement>
    static void binder<Statement, int>::bind(
        Statement& statement,
        int        index,
        int        value
    )
    {
        statement.bind_int64(index, static_cast<std::int64_t>(value));
    }

    template <typename Statement>
    static int binder<Statement, int>::read(Statement const& statement, int col)
    {
        return static_cast<int>(statement.column_int64(col));
    }

    template <typename Statement>
    static void binder<Statement, bool>::bind(
        Statement& statement,
        int        index,
        bool       value
    )
    {
        statement.bind_int64(index, value ? 1 : 0);
    }

    template <typename Statement>
    static bool binder<Statement, bool>::read(
        Statement const& statement,
        int              col
    )
    {
        return statement.column_int64(col) != 0;
    }

    template <typename Statement>
    static void binder<Statement, double>::bind(
        Statement& statement,
        int        index,
        double     value
    )
    {
        statement.bind_double(index, value);
    }

    template <typename Statement>
    static double binder<Statement, double>::read(
        Statement const& statement,
        int              col
    )
    {
        return statement.column_double(col);
    }

    template <typename Statement>
    static void binder<Statement, std::string>::bind(
        Statement&         statement,
        int                index,
        std::string const& value
    )
    {
        statement.bind_text(index, value);
    }

    template <typename Statement>
    static std::string binder<Statement, std::string>::read(
        Statement const& statement,
        int              col
    )
    {
        return statement.column_text(col);
    }

    template <typename Statement, typename T>
    static void binder<Statement, T>::bind(
        Statement& statement,
        int        index,
        T const&   value
    )
    requires strong_id<T>
    {
        statement.bind_int64(index, value.value());
    }

    template <typename Statement, typename T>
    static void binder<Statement, T>::bind(
        Statement& statement,
        int        index,
        T const&   value
    )
    requires strong_id<T>
    {
        statement.bind_int64(index, value.value());
    }

    template <typename Statement, strong_id T>
    static T binder<Statement, T>::read(Statement const& statement, int col)
    {
        return T::from(statement.column_int64(col));
    }
}   // namespace orm

#endif   // __ORM__BINDER_TPP__