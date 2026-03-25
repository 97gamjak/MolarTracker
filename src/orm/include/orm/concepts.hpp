#ifndef __ORM__CONCEPTS_HPP__
#define __ORM__CONCEPTS_HPP__

#include <concepts>
#include <cstdint>
#include <optional>

#include "orm/constraints.hpp"

namespace orm
{
    /**
     * @brief Concept for strong ID types
     *
     * A strong ID type must have a method `value()` that returns its underlying
     * integer value, and a static method `from(int64_t)` that constructs an
     * instance from an integer value.
     *
     * @tparam T
     */
    template <typename T>
    concept strong_id = requires(T value) {
        { value.value() } -> std::same_as<std::int64_t>;
        { T::from(std::int64_t{}) } -> std::same_as<T>;
    };

    // TODO(97gamjak): migrate has_option_v and ... to mstd
    // https://97gamjak.atlassian.net/browse/MOLTRACK-85

    /**
     * @brief Check if a type is among the options
     *
     * @tparam Wanted
     * @tparam Options
     */
    template <typename Wanted, typename... Options>
    inline constexpr bool has_option_v =
        (std::is_same_v<Wanted, Options> || ...);

    /**
     * @brief Concept for nullable types
     *
     */
    template <typename T>
    struct is_optional : std::false_type
    {
    };

    /**
     * @brief Specialization for std::optional
     *
     * @tparam U
     */
    template <typename U>
    struct is_optional<std::optional<U>> : std::true_type
    {
    };

    /**
     * @brief Variable template for is_optional
     *
     * @tparam T
     */
    template <typename T>
    inline constexpr bool is_optional_v = is_optional<T>::value;

    /**
     * @brief Extract the inner type from an optional type
     *
     * @tparam T
     */
    template <typename T>
    struct optional_inner
    {
        /// Type alias for the inner type, which is just T for non-optional
        /// types
        using type = T;
    };

    /**
     * @brief Specialization for std::optional
     *
     * @tparam U
     */
    template <typename U>
    struct optional_inner<std::optional<U>>
    {
        /// Type alias for the inner type, which is U for std::optional<U>
        using type = U;
    };

    /**
     * @brief Type alias for optional_inner
     *
     * @tparam T
     */
    template <typename T>
    using optional_inner_t = typename optional_inner<T>::type;

    /**
     * @brief Check if a value type is nullable given the options
     *
     * @tparam ValueType
     * @tparam Options
     */
    template <typename ValueType, typename... Options>
    inline constexpr bool is_nullable_v =
        has_option_v<nullable_t, Options...> ||
        (is_optional_v<ValueType> && !has_option_v<not_null_t, Options...>);

    /**
     * @brief Check if any of the options is a foreign key constraint for the
     * given value type
     *
     * @tparam T
     */
    template <typename T>
    struct is_any_foreign_key : std::false_type
    {
    };

    /**
     * @brief Specialization for foreign_key_t
     *
     * @tparam DeleteType
     * @tparam TableType
     * @tparam FieldType
     */
    template <typename DeleteType, typename TableType, typename FieldType>
    struct is_any_foreign_key<foreign_key_t<DeleteType, TableType, FieldType>>
        : std::true_type
    {
    };

    /**
     * @brief Variable template for is_any_foreign_key
     *
     * @tparam T
     */
    template <typename ValueType, typename... Options>
    inline constexpr bool has_foreign_key_v =
        (is_any_foreign_key<Options>::value || ...);

    /**
     * @brief Find the foreign key option for a given value type among the
     * options, this is used to extract the foreign key information for a field
     * when generating the SQL for a table, if there are multiple foreign key
     * options for the same value type, the first one will be used, if there is
     * no foreign key option for the value type, the type will be void
     *
     * @tparam ValueType
     * @tparam Options
     */
    template <typename... Options>
    struct find_foreign_key
    {
        /// fallback if not found
        using type = void;
    };

    /**
     * @brief Recursive specialization to find the foreign key option for a
     * given value type among the options, this is used to extract the foreign
     * key information for a field when generating the SQL for a table, if there
     * are multiple foreign key options for the same value type, the first one
     * will be used
     *
     * @tparam ValueType
     * @tparam Options
     */
    template <typename Head, typename... Tail>
    struct find_foreign_key<Head, Tail...>
    {
        /// Type alias for the found foreign key option
        using type = std::conditional_t<
            is_any_foreign_key<Head>::value,
            Head,
            typename find_foreign_key<Tail...>::type>;
    };

    /**
     * @brief Type alias for find_foreign_key
     *
     * @tparam ValueType
     * @tparam Options
     */
    template <typename... Options>
    using find_foreign_key_t = typename find_foreign_key<Options...>::type;

}   // namespace orm

#endif   // __ORM__CONCEPTS_HPP__