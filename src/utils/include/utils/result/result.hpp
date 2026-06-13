#ifndef __UTILS__INCLUDE__UTILS__RESULT__RESULT_HPP__
#define __UTILS__INCLUDE__UTILS__RESULT__RESULT_HPP__

#include <expected>
#include <type_traits>

#include "result_tags.hpp"

/**
 * @brief Models any Result-like type that exposes value_type and error_type.
 */
template <typename R>
concept ResultLike = requires {
    typename R::value_type;
    typename R::error_type;
};

/**
 * @brief A wrapper around std::expected<T, E> with richer monadic operations.
 *
 * Adds over the raw std::expected interface:
 *   - inspect / inspect_error for side-effect-only chains
 *   - flatten for Result<Result<T,E>,E> → Result<T,E>
 *   - map / map_error as aliases for transform / transform_error
 *   - unwrap family (unwrap, unwrap_or, unwrap_or_else)
 *
 * All monadic ops return Result<U,E>, not std::expected<U,E>, so chains stay
 * in Result land.
 *
 * @tparam T Value type (must not be a reference)
 * @tparam E Error type (must not be a reference)
 */
template <typename T, typename E>
class Result
{
    static_assert(
        !std::is_reference_v<T>,
        "Result value type cannot be a reference"
    );
    static_assert(
        !std::is_reference_v<E>,
        "Result error type cannot be a reference"
    );

    std::expected<T, E> _inner;

   public:
    using value_type = T;
    using error_type = E;

    // NOLINTBEGIN(google-explicit-constructor, hicpp-explicit-conversions)
    Result(Ok<T> object);
    Result(Err<E> object);
    Result(std::expected<T, E> exp);
    // NOLINTEND(google-explicit-constructor, hicpp-explicit-conversions)

    // ── Observers ────────────────────────────────────────────────────────────

    [[nodiscard]] bool has_value() const;
    explicit           operator bool() const;

    [[nodiscard]] T&       operator*() &;
    [[nodiscard]] const T& operator*() const&;
    [[nodiscard]] T&&      operator*() &&;

    [[nodiscard]] T*       operator->();
    [[nodiscard]] const T* operator->() const;

    [[nodiscard]] T&       value() &;
    [[nodiscard]] const T& value() const&;
    [[nodiscard]] T&&      value() &&;

    [[nodiscard]] E&       error() &;
    [[nodiscard]] const E& error() const&;
    [[nodiscard]] E&&      error() &&;

    // ── unwrap ───────────────────────────────────────────────────────────────
    //
    // Swap std::runtime_error for MolarException if you want a captured
    // stacktrace on panic (unwrap is a programming error, not a user error).

    [[nodiscard]] T unwrap() &&;

    [[nodiscard]] T unwrap_or(T fallback) &&;

    template <std::invocable<const E&> F>
    requires std::convertible_to<std::invoke_result_t<F, const E&>, T>
    [[nodiscard]] T unwrap_or_else(F&& func) &&;

    // ── Inspection ───────────────────────────────────────────────────────────
    //
    // Side-effect only; returns self so chains on temporaries work:
    //   makeResult().inspect(...).and_then(...).unwrap()

    template <std::invocable<const T&> F>
    Result& inspect(F&& func) &;

    template <std::invocable<const T&> F>
    Result&& inspect(F&& func) &&;

    template <std::invocable<const E&> F>
    Result& inspect_error(F&& func) &;

    template <std::invocable<const E&> F>
    Result&& inspect_error(F&& func) &&;

    // ── Monadic ops ──────────────────────────────────────────────────────────

    // and_then: (T -> Result<U, E>) -> Result<U, E>
    template <std::invocable<T> F>
    requires ResultLike<std::invoke_result_t<F, T>> &&
             std::same_as<typename std::invoke_result_t<F, T>::error_type, E>
    [[nodiscard]] auto and_then(F&& func) &&;

    // transform / map: (T -> U) -> Result<U, E>
    // Handles U = void — transforms into Result<void, E>.
    template <std::invocable<T> F>
    [[nodiscard]] auto transform(F&& func) &&;

    template <std::invocable<T> F>
    [[nodiscard]] auto map(F&& func) &&;

    // transform_error / map_error: (E -> E2) -> Result<T, E2>
    template <std::invocable<E> F>
    [[nodiscard]] auto transform_error(F&& func) &&;

    template <std::invocable<E> F>
    [[nodiscard]] auto map_error(F&& func) &&;

    // or_else: (E -> Result<T, E2>) -> Result<T, E2>
    template <std::invocable<E> F>
    requires ResultLike<std::invoke_result_t<F, E>> &&
             std::same_as<typename std::invoke_result_t<F, E>::value_type, T>
    [[nodiscard]] auto or_else(F&& func) &&;

    // flatten: Result<Result<T, E>, E> -> Result<T, E>
    [[nodiscard]] auto flatten() &&
    requires ResultLike<T> && std::same_as<typename T::error_type, E>;
};

// ─── Void specialization
// ──────────────────────────────────────────────────────

/**
 * @brief Result<void, E> — sugar for operations that succeed with no value.
 *
 * Adds then(func) as the primary primitive for sequencing void operations or
 * graduating into a value-carrying chain:
 *
 *   openDb()                             // Result<void, Error>
 *       .then([&]{ return loadUser(); }) // Result<User, Error>
 *       .and_then([](User u) { ... })
 *
 * @tparam E Error type
 */
template <typename E>
class Result<void, E>
{
    std::expected<void, E> _inner;

   public:
    using value_type = void;
    using error_type = E;

    // NOLINTBEGIN(google-explicit-constructor, hicpp-explicit-conversions)
    Result(Ok<void> /*ok*/);
    Result(Err<E> error);
    Result(std::expected<void, E> exp);
    // NOLINTEND(google-explicit-constructor, hicpp-explicit-conversions)

    [[nodiscard]] bool has_value() const;
    explicit           operator bool() const;

    void value() const;

    [[nodiscard]] E&       error() &;
    [[nodiscard]] const E& error() const&;
    [[nodiscard]] E&&      error() &&;

    void unwrap() &&;

    template <std::invocable F>
    Result& inspect(F&& func) &;

    template <std::invocable F>
    Result&& inspect(F&& func) &&;

    template <std::invocable<const E&> F>
    Result& inspect_error(F&& func) &;

    template <std::invocable<const E&> F>
    Result&& inspect_error(F&& func) &&;

    template <std::invocable F>
    [[nodiscard]] auto then(F&& func) &&;

    template <std::invocable F>
    [[nodiscard]] auto transform(F&& func) &&;

    template <std::invocable F>
    requires ResultLike<std::invoke_result_t<F>> &&
             std::same_as<typename std::invoke_result_t<F>::error_type, E>
    [[nodiscard]] auto and_then(F&& func) &&;

    template <std::invocable<E> F>
    requires ResultLike<std::invoke_result_t<F, E>> &&
             std::is_void_v<typename std::invoke_result_t<F, E>::value_type>
    [[nodiscard]] auto or_else(F&& func) &&;

    template <std::invocable<E> F>
    [[nodiscard]] auto transform_error(F&& func) &&;

    template <std::invocable<E> F>
    [[nodiscard]] auto map_error(F&& func) &&;
};

#ifndef __UTILS__INCLUDE__UTILS__RESULT__RESULT_TPP__
#include "result.tpp"
#endif   // __UTILS__INCLUDE__UTILS__RESULT__RESULT_TPP__

#endif   // __UTILS__INCLUDE__UTILS__RESULT__RESULT_HPP__
