#ifndef __RESULT__INCLUDE__RESULT__RESULT_HPP__
#define __RESULT__INCLUDE__RESULT__RESULT_HPP__

#include <concepts>
#include <expected>
#include <format>
#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

// ─── detail ──────────────────────────────────────────────────────────────────

namespace detail
{

    /**
     * @brief Format an error value for unwrap() panic messages.
     *
     * Falls back gracefully if E is not std::formattable or an exception type.
     */
    template <typename E>
    [[nodiscard]] std::string format_error(const E& err)
    {
        if constexpr (requires { std::format("{}", err); })
            return std::format("unwrap() on Err: {}", err);
        else if constexpr (requires { err.what(); })
            return std::format("unwrap() on Err: {}", err.what());
        else
            return "unwrap() on Err: <non-formattable error type>";
    }

}   // namespace detail

// ─── Tag types
// ────────────────────────────────────────────────────────────────

/**
 * @brief Tag struct representing a successful Result value.
 *
 * @tparam T The value type (use Ok<void> for void results)
 */
template <typename T>
struct Ok
{
    /// The wrapped success value
    T value;

    explicit Ok(T v) noexcept(std::is_nothrow_move_constructible_v<T>)
        : value(std::move(v))
    {
    }
};

template <>
struct Ok<void>
{
};

template <typename T>
Ok(T) -> Ok<T>;

/**
 * @brief Tag struct representing a failed Result value.
 *
 * @tparam E The error type
 */
template <typename E>
struct Err
{
    /// The wrapped error
    E error;

    explicit Err(E e) noexcept(std::is_nothrow_move_constructible_v<E>)
        : error(std::move(e))
    {
    }
};

template <typename E>
Err(E) -> Err<E>;

/**
 * @brief Factory for Result<void, E> success paths.
 */
[[nodiscard]] inline Ok<void> ok() noexcept { return {}; }

// ─── ResultLike concept
// ───────────────────────────────────────────────────────

/**
 * @brief Models any Result-like type that exposes value_type and error_type.
 */
template <typename R>
concept ResultLike = requires {
    typename R::value_type;
    typename R::error_type;
};

// ─── Forward declaration
// ──────────────────────────────────────────────────────

template <typename T, typename E>
class Result;

// ─── Primary template
// ─────────────────────────────────────────────────────────

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

    // ── Construction ─────────────────────────────────────────────────────────

    // NOLINTNEXTLINE(google-explicit-constructor)
    Result(Ok<T> o) noexcept(std::is_nothrow_move_constructible_v<T>)
        : _inner(std::move(o.value))
    {
    }

    // NOLINTNEXTLINE(google-explicit-constructor)
    Result(Err<E> e) noexcept(std::is_nothrow_move_constructible_v<E>)
        : _inner(std::unexpected(std::move(e.error)))
    {
    }

    // NOLINTNEXTLINE(google-explicit-constructor)
    Result(std::expected<T, E> exp) : _inner(std::move(exp)) {}

    // ── Observers ────────────────────────────────────────────────────────────

    [[nodiscard]] bool has_value() const noexcept { return _inner.has_value(); }
    explicit operator bool() const noexcept { return _inner.has_value(); }

    [[nodiscard]] T&       operator*() & noexcept { return *_inner; }
    [[nodiscard]] const T& operator*() const& noexcept { return *_inner; }
    [[nodiscard]] T&& operator*() && noexcept { return *std::move(_inner); }

    [[nodiscard]] T*       operator->() noexcept { return _inner.operator->(); }
    [[nodiscard]] const T* operator->() const noexcept
    {
        return _inner.operator->();
    }

    [[nodiscard]] T&       value() & { return _inner.value(); }
    [[nodiscard]] const T& value() const& { return _inner.value(); }
    [[nodiscard]] T&&      value() && { return std::move(_inner).value(); }

    [[nodiscard]] E&       error() & noexcept { return _inner.error(); }
    [[nodiscard]] const E& error() const& noexcept { return _inner.error(); }
    [[nodiscard]] E&& error() && noexcept { return std::move(_inner).error(); }

    // ── unwrap ───────────────────────────────────────────────────────────────
    //
    // Swap std::runtime_error for MolarException if you want a captured
    // stacktrace on panic (unwrap is a programming error, not a user error).

    [[nodiscard]] T unwrap() &&
    {
        if (!_inner.has_value())
            throw std::runtime_error(detail::format_error(_inner.error()));
        return std::move(*_inner);
    }

    [[nodiscard]] T unwrap_or(T fallback) &&
    {
        return _inner.has_value() ? std::move(*_inner) : std::move(fallback);
    }

    template <std::invocable<const E&> F>
    requires std::convertible_to<std::invoke_result_t<F, const E&>, T>
    [[nodiscard]] T unwrap_or_else(F&& fn) &&
    {
        if (_inner.has_value())
            return std::move(*_inner);
        return static_cast<T>(std::invoke(std::forward<F>(fn), _inner.error()));
    }

    // ── Inspection ───────────────────────────────────────────────────────────
    //
    // Side-effect only; returns self so chains on temporaries work:
    //   makeResult().inspect(...).and_then(...).unwrap()

    template <std::invocable<const T&> F>
    Result& inspect(F&& fn) &
    {
        if (_inner.has_value())
            std::invoke(std::forward<F>(fn), *_inner);
        return *this;
    }

    template <std::invocable<const T&> F>
    Result&& inspect(F&& fn) &&
    {
        if (_inner.has_value())
            std::invoke(std::forward<F>(fn), *_inner);
        return std::move(*this);
    }

    template <std::invocable<const E&> F>
    Result& inspect_error(F&& fn) &
    {
        if (!_inner.has_value())
            std::invoke(std::forward<F>(fn), _inner.error());
        return *this;
    }

    template <std::invocable<const E&> F>
    Result&& inspect_error(F&& fn) &&
    {
        if (!_inner.has_value())
            std::invoke(std::forward<F>(fn), _inner.error());
        return std::move(*this);
    }

    // ── Monadic ops ──────────────────────────────────────────────────────────

    // and_then: (T -> Result<U, E>) -> Result<U, E>
    template <std::invocable<T> F>
    requires ResultLike<std::invoke_result_t<F, T>> &&
             std::same_as<typename std::invoke_result_t<F, T>::error_type, E>
    [[nodiscard]] auto and_then(F&& fn) &&
    {
        using Ret = std::invoke_result_t<F, T>;
        if (_inner.has_value())
            return std::invoke(std::forward<F>(fn), std::move(*_inner));
        return Ret{Err<E>{std::move(_inner).error()}};
    }

    // transform / map: (T -> U) -> Result<U, E>
    // Handles U = void — transforms into Result<void, E>.
    template <std::invocable<T> F>
    [[nodiscard]] auto transform(F&& fn) &&
    {
        using U = std::invoke_result_t<F, T>;
        if constexpr (std::is_void_v<U>)
        {
            if (_inner.has_value())
            {
                std::invoke(std::forward<F>(fn), std::move(*_inner));
                return Result<void, E>{ok()};
            }
            return Result<void, E>{Err<E>{std::move(_inner).error()}};
        }
        else
        {
            if (_inner.has_value())
                return Result<U, E>{
                    Ok<U>{std::invoke(std::forward<F>(fn), std::move(*_inner))}
                };
            return Result<U, E>{Err<E>{std::move(_inner).error()}};
        }
    }

    template <std::invocable<T> F>
    [[nodiscard]] auto map(F&& fn) &&
    {
        return std::move(*this).transform(std::forward<F>(fn));
    }

    // transform_error / map_error: (E -> E2) -> Result<T, E2>
    template <std::invocable<E> F>
    [[nodiscard]] auto transform_error(F&& fn) &&
    {
        using E2 = std::invoke_result_t<F, E>;
        if (!_inner.has_value())
            return Result<T, E2>{Err<E2>{
                std::invoke(std::forward<F>(fn), std::move(_inner).error())
            }};
        return Result<T, E2>{Ok<T>{std::move(*_inner)}};
    }

    template <std::invocable<E> F>
    [[nodiscard]] auto map_error(F&& fn) &&
    {
        return std::move(*this).transform_error(std::forward<F>(fn));
    }

    // or_else: (E -> Result<T, E2>) -> Result<T, E2>
    template <std::invocable<E> F>
    requires ResultLike<std::invoke_result_t<F, E>> &&
             std::same_as<typename std::invoke_result_t<F, E>::value_type, T>
    [[nodiscard]] auto or_else(F&& fn) &&
    {
        using Ret = std::invoke_result_t<F, E>;
        if (!_inner.has_value())
            return std::invoke(std::forward<F>(fn), std::move(_inner).error());
        return Ret{Ok<T>{std::move(*_inner)}};
    }

    // flatten: Result<Result<T, E>, E> -> Result<T, E>
    [[nodiscard]] auto flatten() &&
    requires ResultLike<T> && std::same_as<typename T::error_type, E>
    {
        if (_inner.has_value())
            return std::move(*_inner);
        return T{Err<E>{std::move(_inner).error()}};
    }
};

// ─── Void specialization
// ──────────────────────────────────────────────────────

/**
 * @brief Result<void, E> — sugar for operations that succeed with no value.
 *
 * Adds then(fn) as the primary primitive for sequencing void operations or
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

    // NOLINTNEXTLINE(google-explicit-constructor)
    Result(Ok<void>) noexcept : _inner() {}

    // NOLINTNEXTLINE(google-explicit-constructor)
    Result(Err<E> e) noexcept(std::is_nothrow_move_constructible_v<E>)
        : _inner(std::unexpected(std::move(e.error)))
    {
    }

    // NOLINTNEXTLINE(google-explicit-constructor)
    Result(std::expected<void, E> exp) : _inner(std::move(exp)) {}

    [[nodiscard]] bool has_value() const noexcept { return _inner.has_value(); }
    explicit operator bool() const noexcept { return _inner.has_value(); }

    void value() const { _inner.value(); }

    [[nodiscard]] E&       error() & noexcept { return _inner.error(); }
    [[nodiscard]] const E& error() const& noexcept { return _inner.error(); }
    [[nodiscard]] E&& error() && noexcept { return std::move(_inner).error(); }

    void unwrap() &&
    {
        if (!_inner.has_value())
            throw std::runtime_error(detail::format_error(_inner.error()));
    }

    template <std::invocable F>
    Result& inspect(F&& fn) &
    {
        if (_inner.has_value())
            std::invoke(std::forward<F>(fn));
        return *this;
    }

    template <std::invocable F>
    Result&& inspect(F&& fn) &&
    {
        if (_inner.has_value())
            std::invoke(std::forward<F>(fn));
        return std::move(*this);
    }

    template <std::invocable<const E&> F>
    Result& inspect_error(F&& fn) &
    {
        if (!_inner.has_value())
            std::invoke(std::forward<F>(fn), _inner.error());
        return *this;
    }

    template <std::invocable<const E&> F>
    Result&& inspect_error(F&& fn) &&
    {
        if (!_inner.has_value())
            std::invoke(std::forward<F>(fn), _inner.error());
        return std::move(*this);
    }

    // then / transform: (() -> U) -> Result<U, E>
    template <std::invocable F>
    [[nodiscard]] auto then(F&& fn) &&
    {
        using U = std::invoke_result_t<F>;
        if constexpr (std::is_void_v<U>)
        {
            if (_inner.has_value())
            {
                std::invoke(std::forward<F>(fn));
                return Result<void, E>{ok()};
            }
            return Result<void, E>{Err<E>{std::move(_inner).error()}};
        }
        else
        {
            if (_inner.has_value())
                return Result<U, E>{Ok<U>{std::invoke(std::forward<F>(fn))}};
            return Result<U, E>{Err<E>{std::move(_inner).error()}};
        }
    }

    template <std::invocable F>
    [[nodiscard]] auto transform(F&& fn) &&
    {
        return std::move(*this).then(std::forward<F>(fn));
    }

    template <std::invocable F>
    requires ResultLike<std::invoke_result_t<F>> &&
             std::same_as<typename std::invoke_result_t<F>::error_type, E>
    [[nodiscard]] auto and_then(F&& fn) &&
    {
        using Ret = std::invoke_result_t<F>;
        if (_inner.has_value())
            return std::invoke(std::forward<F>(fn));
        return Ret{Err<E>{std::move(_inner).error()}};
    }

    template <std::invocable<E> F>
    requires ResultLike<std::invoke_result_t<F, E>> &&
             std::is_void_v<typename std::invoke_result_t<F, E>::value_type>
    [[nodiscard]] auto or_else(F&& fn) &&
    {
        using Ret = std::invoke_result_t<F, E>;
        if (!_inner.has_value())
            return std::invoke(std::forward<F>(fn), std::move(_inner).error());
        return Ret{ok()};
    }

    template <std::invocable<E> F>
    [[nodiscard]] auto transform_error(F&& fn) &&
    {
        using E2 = std::invoke_result_t<F, E>;
        if (!_inner.has_value())
            return Result<void, E2>{Err<E2>{
                std::invoke(std::forward<F>(fn), std::move(_inner).error())
            }};
        return Result<void, E2>{ok()};
    }

    template <std::invocable<E> F>
    [[nodiscard]] auto map_error(F&& fn) &&
    {
        return std::move(*this).transform_error(std::forward<F>(fn));
    }
};

#endif   // __RESULT__INCLUDE__RESULT__RESULT_HPP__
