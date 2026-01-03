#ifndef __ORM__CONCEPTS_HPP__
#define __ORM__CONCEPTS_HPP__

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

    template <typename Wanted, typename... Options>
    inline constexpr bool has_option_v =
        (std::is_same_v<Wanted, Options> || ...);

    template <typename T>
    struct is_optional : std::false_type
    {
    };

    template <typename U>
    struct is_optional<std::optional<U>> : std::true_type
    {
    };

    template <typename T>
    inline constexpr bool is_optional_v = is_optional<T>::value;

    template <typename T>
    struct optional_inner
    {
        using type = T;
    };

    template <typename U>
    struct optional_inner<std::optional<U>>
    {
        using type = U;
    };

    template <typename T>
    using optional_inner_t = typename optional_inner<T>::type;

    template <typename ValueType, typename... Options>
    inline constexpr bool is_nullable_v =
        has_option_v<nullable_t, Options...> ||
        (is_optional_v<ValueType> && !has_option_v<not_null_t, Options...>);

}   // namespace orm

#endif   // __ORM__CONCEPTS_HPP__