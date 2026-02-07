#ifndef __ORM__SQL_TYPE_HPP__
#define __ORM__SQL_TYPE_HPP__

#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>

#include "concepts.hpp"

namespace orm
{
    template <typename T>
    struct sql_type;

    // clang-format off
    template <> struct sql_type<std::int64_t>{static constexpr std::string_view name = "INTEGER";};
    template <> struct sql_type<int>         {static constexpr std::string_view name = "INTEGER";};
    template <> struct sql_type<bool>        {static constexpr std::string_view name = "INTEGER";};
    template <> struct sql_type<double>      {static constexpr std::string_view name = "REAL";};
    template <> struct sql_type<std::string> {static constexpr std::string_view name = "TEXT";};
    // clang-format on

    template <strong_id T>
    struct sql_type<T>
    {
        static constexpr std::string_view name = "INTEGER";
    };
}   // namespace orm

#endif   // __ORM__SQL_TYPE_HPP__