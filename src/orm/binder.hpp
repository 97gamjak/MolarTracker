#ifndef __ORM__BINDER_HPP__
#define __ORM__BINDER_HPP__

#include <cstdint>
#include <string>
#include <string_view>

#include "sqlite_type.hpp"

namespace orm
{
    template <typename Statement, typename T>
    struct binder;

    template <typename Statement>
    struct binder<Statement, std::int64_t>
    {
        static void bind(Statement& statement, int index, std::int64_t value);

        static std::int64_t read(Statement const& statement, int col);
    };

    template <typename Statement>
    struct binder<Statement, int>
    {
        static void bind(Statement& statement, int index, int value);

        static int read(Statement const& statement, int col);
    };

    template <typename Statement>
    struct binder<Statement, bool>
    {
        static void bind(Statement& statement, int index, bool value);

        static bool read(Statement const& statement, int col);
    };

    template <typename Statement>
    struct binder<Statement, double>
    {
        static void bind(Statement& statement, int index, double value);

        static double read(Statement const& statement, int col);
    };

    template <typename Statement>
    struct binder<Statement, std::string>
    {
        static void bind(
            Statement&         statement,
            int                index,
            std::string const& value
        );

        static std::string read(Statement const& statement, int col);
    };

    template <typename Statement, strong_id T>
    struct binder<Statement, T>
    {
        static void bind(Statement& statement, int index, T const& value);

        static T read(Statement const& statement, int col);
    };
}   // namespace orm

#ifndef __ORM__BINDER_TPP__
#include "binder.tpp"
#endif   // __ORM__BINDER_TPP__

#endif   // __ORM__BINDER_HPP__