#ifndef __ORM__SQL_TYPE_HPP__
#define __ORM__SQL_TYPE_HPP__

#include <cstdint>
#include <string>
#include <string_view>

#include "concepts.hpp"

namespace orm
{
    /**
     * @brief A template struct to map C++ types to their corresponding SQL
     * types.
     *
     * This struct is specialized for fundamental types and strong_id types to
     * provide the appropriate SQL type name as a string view. The name member
     * can be used in SQL queries to specify the type of a column or parameter.
     *
     * @tparam T The C++ type to map to an SQL type.
     */
    template <typename T>
    struct sql_type;

    /**
     * @brief Specialization of sql_type for std::int64_t
     *
     * @tparam std::int64_t The C++ type to specialize for, which is
     * std::int64_t in this case.
     */
    template <>
    struct sql_type<std::int64_t>
    {
        /// SQLITE mapping for 64-bit integers is INTEGER
        static constexpr std::string_view name = "INTEGER";
    };

    /**
     * @brief Specialization of sql_type for int.
     *
     * @tparam int The C++ type to specialize for, which is int in this case.
     */
    template <>
    struct sql_type<int>
    {
        /// SQLITE mapping for integers is INTEGER
        static constexpr std::string_view name = "INTEGER";
    };

    /**
     * @brief Specialization of sql_type for bool.
     *
     * @tparam bool The C++ type to specialize for, which is bool in this case.
     */
    template <>
    struct sql_type<bool>
    {
        /// SQLITE mapping for boolean values is INTEGER, where 0 is false and 1
        /// is true
        static constexpr std::string_view name = "INTEGER";
    };

    /**
     * @brief Specialization of sql_type for double.
     *
     * @tparam double The C++ type to specialize for, which is double in this
     * case.
     */
    template <>
    struct sql_type<double>
    {
        /// SQLITE mapping for floating-point numbers is REAL
        static constexpr std::string_view name = "REAL";
    };

    /**
     * @brief Specialization of sql_type for std::string.
     *
     * @tparam std::string The C++ type to specialize for, which is
     * std::string in this case.
     */
    template <>
    struct sql_type<std::string>
    {
        /// SQLite mapping for strings is TEXT
        static constexpr std::string_view name = "TEXT";
    };

    /**
     * @brief Specialization of sql_type for strong_id types. Strong IDs are
     * typically represented as integers in the database, so we map them to
     * INTEGER.
     *
     * @tparam T The strong_id type to specialize for.
     */
    template <strong_id T>
    struct sql_type<T>
    {
        /// We assume that strong_id types are represented as integers in the
        /// database, so we use INTEGER as the SQL type
        static constexpr std::string_view name = "INTEGER";
    };

}   // namespace orm

#endif   // __ORM__SQL_TYPE_HPP__