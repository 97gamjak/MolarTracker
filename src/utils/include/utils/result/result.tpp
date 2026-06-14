#ifndef __UTILS__INCLUDE__UTILS__RESULT__RESULT_TPP__
#define __UTILS__INCLUDE__UTILS__RESULT__RESULT_TPP__

#include <format>
#include <functional>

#include "result.hpp"

namespace detail
{
    /**
     * @brief Format an error value for unwrap() panic messages.
     *
     * Falls back gracefully if E is not std::formattable or an exception type.
     */
    template <typename E, typename Exception>
    [[nodiscard]] std::string format_error(const E& err)
    {
        if constexpr (requires { std::format("{}", err); })
        {
            return std::format("unwrap() on Err: {}", err);
        }
        else if constexpr (requires { err.what(); })
        {
            return std::format("unwrap() on Err: {}", err.what());
        }
        else
        {
            return "unwrap() on Err: <non-formattable error type>";
        }
    }
}   // namespace detail

template <typename T, typename E, typename Exception>
Result<T, E, Exception>::Result(T value) : _inner(std::move(value))
{
}

/**
 * @brief Construct a new Result< T,  E>:: Result object
 *
 * @tparam T
 * @tparam E
 * @param object
 */
template <typename T, typename E, typename Exception>
Result<T, E, Exception>::Result(Ok<T> object) : _inner(std::move(object.value))
{
}

/**
 * @brief Construct a new Result< T,  E>:: Result object
 *
 * @tparam T
 * @tparam E
 * @param object
 */
template <typename T, typename E, typename Exception>
Result<T, E, Exception>::Result(Err<E> object)
    : _inner(std::unexpected(std::move(object.error)))
{
}

/**
 * @brief Construct a new Result< T,  E>:: Result object
 *
 * @tparam T
 * @tparam E
 * @param exp
 */
template <typename T, typename E, typename Exception>
Result<T, E, Exception>::Result(std::expected<T, E> exp)
    : _inner(std::move(exp))
{
}

/**
 * @brief Construct a new Result<void, E, Exception>::Result object
 *
 * @tparam E
 */
template <typename E, typename Exception>
Result<void, E, Exception>::Result(Ok<void> /*ok*/) : _inner()
{
}

/**
 * @brief Construct a new Result<void, E, Exception>::Result object
 *
 * @tparam E
 * @param error
 */
template <typename E, typename Exception>
Result<void, E, Exception>::Result(Err<E> error)
    : _inner(std::unexpected(std::move(error.error)))
{
}

/**
 * @brief Construct a new Result<void, E, Exception>::Result object
 *
 * @tparam E
 * @param exp
 */
template <typename E, typename Exception>
Result<void, E, Exception>::Result(std::expected<void, E> exp)
    : _inner(std::move(exp))
{
}

/**
 * @brief Check if the Result has a value (is Ok) or an error (is Err).
 *
 * @tparam T
 * @tparam E
 * @return true
 * @return false
 */
template <typename T, typename E, typename Exception>
bool Result<T, E, Exception>::has_value() const
{
    return _inner.has_value();
}

/**
 * @brief Check if the Result has a value (is Ok) or an error (is Err).
 *
 * @tparam E
 * @return true
 * @return false
 */
template <typename E, typename Exception>
bool Result<void, E, Exception>::has_value() const
{
    return _inner.has_value();
}

/**
 * @brief Check if the Result has a value (is Ok) or an error (is Err).
 *
 * @tparam T
 * @tparam E
 * @return true
 * @return false
 */
template <typename T, typename E, typename Exception>
Result<T, E, Exception>::operator bool() const
{
    return _inner.has_value();
}

/**
 * @brief Check if the Result has a value (is Ok) or an error (is Err).
 *
 * @tparam E
 * @return true
 * @return false
 */
template <typename E, typename Exception>
Result<void, E, Exception>::operator bool() const
{
    return _inner.has_value();
}

/**
 * @brief Dereference the Result to get the value (if Ok) or throw (if Err).
 *
 * @tparam T
 * @tparam E
 * @return T&
 */
template <typename T, typename E, typename Exception>
T& Result<T, E, Exception>::operator*() &
{
    return *_inner;
}

/**
 * @brief Dereference the Result to get the value (if Ok) or throw (if Err).
 *
 * @tparam T
 * @tparam E
 * @return const T&
 */
template <typename T, typename E, typename Exception>
const T& Result<T, E, Exception>::operator*() const&
{
    return *_inner;
}

/**
 * @brief Dereference the Result to get the value (if Ok) or throw (if Err).
 *
 * @tparam T
 * @tparam E
 * @return T&&
 */
template <typename T, typename E, typename Exception>
T&& Result<T, E, Exception>::operator*() &&
{
    return *std::move(_inner);
}

/**
 * @brief Dereference the Result to get the value (if Ok) or throw (if Err).
 *
 * @tparam T
 * @tparam E
 * @return T*
 */
template <typename T, typename E, typename Exception>
T* Result<T, E, Exception>::operator->()
{
    return _inner.operator->();
}

/**
 * @brief Dereference the Result to get the value (if Ok) or throw (if Err).
 *
 * @tparam T
 * @tparam E
 * @return const T*
 */
template <typename T, typename E, typename Exception>
const T* Result<T, E, Exception>::operator->() const
{
    return _inner.operator->();
}

/**
 * @brief Get the value (if Ok) or throw (if Err).
 *
 * @tparam T
 * @tparam E
 * @return T&
 */
template <typename T, typename E, typename Exception>
T& Result<T, E, Exception>::value() &
{
    return _inner.value();
}

/**
 * @brief mimic value() for Result<void, E, Exception> since it has no value to
 * return, but we want to be able to call value() on it for uniformity in
 * generic code. Always returns void, but will throw if the Result is an Err.
 *
 * @tparam E
 */
template <typename E, typename Exception>
void Result<void, E, Exception>::value() const
{
    _inner.value();
}

/**
 * @brief Get the value (if Ok) or throw (if Err).
 *
 * @tparam T
 * @tparam E
 * @return const T&
 */
template <typename T, typename E, typename Exception>
const T& Result<T, E, Exception>::value() const&
{
    return _inner.value();
}

/**
 * @brief Get the value (if Ok) or throw (if Err).
 *
 * @tparam T
 * @tparam E
 * @return T&&
 */
template <typename T, typename E, typename Exception>
T&& Result<T, E, Exception>::value() &&
{
    return std::move(_inner).value();
}

/**
 * @brief Get the error (if Err) or throw (if Ok).
 *
 * @tparam T
 * @tparam E
 * @return E&
 */
template <typename T, typename E, typename Exception>
E& Result<T, E, Exception>::error() &
{
    return _inner.error();
}

/**
 * @brief Get the error (if Err) or throw (if Ok).
 *
 * @tparam E
 * @return const E&
 */
template <typename E, typename Exception>
E& Result<void, E, Exception>::error() &
{
    return _inner.error();
}

/**
 * @brief Get the error (if Err) or throw (if Ok).
 *
 * @tparam T
 * @tparam E
 * @return const E&
 */
template <typename T, typename E, typename Exception>
const E& Result<T, E, Exception>::error() const&
{
    return _inner.error();
}

/**
 * @brief Get the error (if Err) or throw (if Ok).
 *
 * @tparam E
 * @return const E&
 */
template <typename E, typename Exception>
const E& Result<void, E, Exception>::error() const&
{
    return _inner.error();
}

/**
 * @brief Get the error (if Err) or throw (if Ok).
 *
 * @tparam T
 * @tparam E
 * @return E&&
 */
template <typename T, typename E, typename Exception>
E&& Result<T, E, Exception>::error() &&
{
    return std::move(_inner).error();
}

/**
 * @brief Get the error (if Err) or throw (if Ok).
 *
 * @tparam E
 * @return E&&
 */
template <typename E, typename Exception>
E&& Result<void, E, Exception>::error() &&
{
    return std::move(_inner).error();
}

/**
 * @brief Unwrap the Result to get the value (if Ok) or throw (if Err).
 *
 * @tparam T
 * @tparam E
 * @return T
 */
template <typename T, typename E, typename Exception>
T Result<T, E, Exception>::unwrap() &&
{
    if (!_inner.has_value())
        throw Exception(detail::format_error(_inner.error()));
    return std::move(*_inner);
}

/**
 * @brief Unwrap the Result to get the value (if Ok) or throw (if Err).
 *
 * @tparam E
 */
template <typename E, typename Exception>
void Result<void, E, Exception>::unwrap() &&
{
    if (!_inner.has_value())
        throw Exception(detail::format_error(_inner.error()));
}

/**
 * @brief Unwrap the Result to get the value (if Ok) or return a fallback (if
 * Err).
 *
 * @tparam T
 * @tparam E
 * @param fallback
 * @return T
 */
template <typename T, typename E, typename Exception>
T Result<T, E, Exception>::unwrap_or(T fallback) &&
{
    return _inner.has_value() ? std::move(*_inner) : std::move(fallback);
}

/**
 * @brief Unwrap the Result to get the value (if Ok) or return a fallback (if
 * Err).
 *
 * @tparam T
 * @tparam E
 * @tparam F
 * @param func
 * @return T
 */
template <typename T, typename E, typename Exception>
template <std::invocable<const E&> F>
requires std::convertible_to<std::invoke_result_t<F, const E&>, T>
T Result<T, E, Exception>::unwrap_or_else(F&& func) &&
{
    if (_inner.has_value())
        return std::move(*_inner);
    return static_cast<T>(std::invoke(std::forward<F>(func), _inner.error()));
}

/// @cond DOXYGEN_IGNORE

/**
 * @brief Inspect the Result to run a function on the value (if Ok) or do
 * nothing (if Err).
 *
 * @tparam T
 * @tparam E
 * @tparam F
 * @param func
 * @return Result<T, E, Exception>&
 */
template <typename T, typename E, typename Exception>
template <std::invocable<const T&> F>
Result<T, E, Exception>& Result<T, E, Exception>::inspect(F&& func) &
{
    if (_inner.has_value())
        std::invoke(std::forward<F>(func), *_inner);
    return *this;
}

/**
 * @brief Inspect the Result to run a function on the value (if Ok) or do
 * nothing (if Err).
 *
 * @tparam E
 * @tparam F
 * @param func
 * @return Result<void, E, Exception>&
 */
template <typename E, typename Exception>
template <std::invocable F>
Result<void, E, Exception>& Result<void, E, Exception>::inspect(F&& func) &
{
    if (_inner.has_value())
        std::invoke(std::forward<F>(func));
    return *this;
}

/**
 * @brief Inspect the Result to run a function on the value (if Ok) or do
 * nothing (if Err).
 *
 * @tparam T
 * @tparam E
 * @tparam F
 * @param func
 * @return Result<T, E, Exception>&&
 */
template <typename T, typename E, typename Exception>
template <std::invocable<const T&> F>
Result<T, E, Exception>&& Result<T, E, Exception>::inspect(F&& func) &&
{
    if (_inner.has_value())
        std::invoke(std::forward<F>(func), *_inner);
    return std::move(*this);
}

/**
 * @brief Inspect the Result to run a function on the value (if Ok) or do
 * nothing (if Err).
 *
 * @tparam E
 * @tparam F
 * @param func
 * @return Result<void, E, Exception>&&
 */
template <typename E, typename Exception>
template <std::invocable F>
Result<void, E, Exception>&& Result<void, E, Exception>::inspect(F&& func) &&
{
    if (_inner.has_value())
        std::invoke(std::forward<F>(func));
    return std::move(*this);
}

/**
 * @brief Inspect the Result to run a function on the error (if Err) or do
 * nothing (if Ok).
 *
 * @tparam T
 * @tparam E
 * @tparam F
 * @param func
 * @return Result<T, E, Exception>&
 */
template <typename T, typename E, typename Exception>
template <std::invocable<const E&> F>
Result<T, E, Exception>& Result<T, E, Exception>::inspect_error(F&& func) &
{
    if (!_inner.has_value())
        std::invoke(std::forward<F>(func), _inner.error());
    return *this;
}

/**
 * @brief Inspect the Result to run a function on the error (if Err) or do
 * nothing (if Ok).
 *
 * @tparam E
 * @tparam F
 * @param func
 * @return Result<void, E, Exception>&
 */
template <typename E, typename Exception>
template <std::invocable<const E&> F>
Result<void, E, Exception>& Result<void, E, Exception>::inspect_error(
    F&& func
) &
{
    if (!_inner.has_value())
        std::invoke(std::forward<F>(func), _inner.error());
    return *this;
}

/**
 * @brief Inspect the Result to run a function on the error (if Err) or do
 * nothing (if Ok).
 *
 * @tparam T
 * @tparam E
 * @tparam F
 * @param func
 * @return Result<T, E, Exception>&&
 */
template <typename T, typename E, typename Exception>
template <std::invocable<const E&> F>
Result<T, E, Exception>&& Result<T, E, Exception>::inspect_error(F&& func) &&
{
    if (!_inner.has_value())
        std::invoke(std::forward<F>(func), _inner.error());
    return std::move(*this);
}

/**
 * @brief Inspect the Result to run a function on the error (if Err) or do
 * nothing (if Ok).
 *
 * @tparam E
 * @tparam F
 * @param func
 * @return Result<void, E, Exception>&&
 */
template <typename E, typename Exception>
template <std::invocable<const E&> F>
Result<void, E, Exception>&& Result<void, E, Exception>::inspect_error(
    F&& func
) &&
{
    if (!_inner.has_value())
        std::invoke(std::forward<F>(func), _inner.error());
    return std::move(*this);
}

/// @endcond

/**
 * @brief Then / transform the Result to run a function on the value (if Ok) or
 * return an Err (if Err).
 *
 * @tparam T
 * @tparam E
 * @tparam F
 * @param func
 * @return auto
 */
template <typename T, typename E, typename Exception>
template <std::invocable<T> F>
requires ResultLike<std::invoke_result_t<F, T>> &&
         std::same_as<typename std::invoke_result_t<F, T>::error_type, E>
auto Result<T, E, Exception>::and_then(F&& func) &&
{
    using Ret = std::invoke_result_t<F, T>;
    if (_inner.has_value())
        return std::invoke(std::forward<F>(func), std::move(*_inner));
    return Ret{Err<E>{std::move(_inner).error()}};
}

/**
 * @brief Then / transform the Result to run a function on the value (if Ok) or
 * return an Err (if Err).
 *
 * @tparam E
 * @tparam F
 * @param func
 * @return auto
 */
template <typename E, typename Exception>
template <std::invocable F>
auto Result<void, E, Exception>::then(F&& func) &&
{
    using U = std::invoke_result_t<F>;
    if constexpr (std::is_void_v<U>)
    {
        if (_inner.has_value())
        {
            std::invoke(std::forward<F>(func));
            return Result<void, E, Exception>{ok()};
        }
        return Result<void, E, Exception>{Err<E>{std::move(_inner).error()}};
    }
    else
    {
        if (_inner.has_value())
            return Result<U, E>{Ok<U>{std::invoke(std::forward<F>(func))}};
        return Result<U, E>{Err<E>{std::move(_inner).error()}};
    }
}

/**
 * @brief Then / transform the Result to run a function on the value (if Ok) or
 * return an Err (if Err).
 *
 * @tparam E
 * @tparam F
 * @param func
 * @return auto
 */
template <typename E, typename Exception>
template <std::invocable F>
requires ResultLike<std::invoke_result_t<F>> &&
         std::same_as<typename std::invoke_result_t<F>::error_type, E>
auto Result<void, E, Exception>::and_then(F&& func) &&
{
    using Ret = std::invoke_result_t<F>;
    if (_inner.has_value())
        return std::invoke(std::forward<F>(func));
    return Ret{Err<E>{std::move(_inner).error()}};
}

/**
 * @brief Transform / map the Result to run a function on the value (if Ok) or
 * return an Err (if Err).
 *
 * @tparam T
 * @tparam E
 * @tparam F
 * @param func
 * @return auto
 */
template <typename T, typename E, typename Exception>
template <std::invocable<T> F>
auto Result<T, E, Exception>::transform(F&& func) &&
{
    using U = std::invoke_result_t<F, T>;
    if constexpr (std::is_void_v<U>)
    {
        if (_inner.has_value())
        {
            std::invoke(std::forward<F>(func), std::move(*_inner));
            return Result<void, E, Exception>{ok()};
        }
        return Result<void, E, Exception>{Err<E>{std::move(_inner).error()}};
    }
    else
    {
        if (_inner.has_value())
            return Result<U, E>{
                Ok<U>{std::invoke(std::forward<F>(func), std::move(*_inner))}
            };
        return Result<U, E>{Err<E>{std::move(_inner).error()}};
    }
}

/**
 * @brief Transform / map the Result to run a function on the value (if Ok) or
 * return an Err (if Err).
 *
 * @tparam E
 * @tparam F
 * @param func
 * @return auto
 */
template <typename E, typename Exception>
template <std::invocable F>
auto Result<void, E, Exception>::transform(F&& func) &&
{
    return std::move(*this).then(std::forward<F>(func));
}

/**
 * @brief Transform / map the Result to run a function on the value (if Ok) or
 * return an Err (if Err).
 *
 * @tparam T
 * @tparam E
 * @tparam F
 * @param func
 * @return auto
 */
template <typename T, typename E, typename Exception>
template <std::invocable<T> F>
auto Result<T, E, Exception>::map(F&& func) &&
{
    return std::move(*this).transform(std::forward<F>(func));
}

/**
 * @brief Transform / map the Result to run a function on the error (if Err) or
 * return an Ok (if Ok).
 *
 * @tparam T
 * @tparam E
 * @tparam F
 * @param func
 * @return auto
 */
template <typename T, typename E, typename Exception>
template <std::invocable<E> F>
auto Result<T, E, Exception>::transform_error(F&& func) &&
{
    using E2 = std::invoke_result_t<F, E>;
    if (!_inner.has_value())
        return Result<T, E2>{Err<E2>{
            std::invoke(std::forward<F>(func), std::move(_inner).error())
        }};
    return Result<T, E2>{Ok<T>{std::move(*_inner)}};
}

/**
 * @brief Transform / map the Result to run a function on the error (if Err) or
 * return an Ok (if Ok).
 *
 * @tparam E
 * @tparam F
 * @param func
 * @return auto
 */
template <typename E, typename Exception>
template <std::invocable<E> F>
auto Result<void, E, Exception>::transform_error(F&& func) &&
{
    using E2 = std::invoke_result_t<F, E>;
    if (!_inner.has_value())
        return Result<void, E2>{Err<E2>{
            std::invoke(std::forward<F>(func), std::move(_inner).error())
        }};
    return Result<void, E2>{ok()};
}

/**
 * @brief Transform / map the Result to run a function on the error (if Err) or
 * return an Ok (if Ok).
 *
 * @tparam T
 * @tparam E
 * @tparam F
 * @param func
 * @return auto
 */
template <typename T, typename E, typename Exception>
template <std::invocable<E> F>
auto Result<T, E, Exception>::map_error(F&& func) &&
{
    return std::move(*this).transform_error(std::forward<F>(func));
}

/**
 * @brief Transform / map the Result to run a function on the error (if Err) or
 * return an Ok (if Ok).
 *
 * @tparam E
 * @tparam F
 * @param func
 * @return auto
 */
template <typename E, typename Exception>
template <std::invocable<E> F>
auto Result<void, E, Exception>::map_error(F&& func) &&
{
    return std::move(*this).transform_error(std::forward<F>(func));
}

/**
 * @brief Or else / transform the Result to run a function on the error (if Err)
 * or return an Ok (if Ok).
 *
 * @tparam T
 * @tparam E
 * @tparam F
 * @param func
 * @return auto
 */
template <typename T, typename E, typename Exception>
template <std::invocable<E> F>
requires ResultLike<std::invoke_result_t<F, E>> &&
         std::same_as<typename std::invoke_result_t<F, E>::value_type, T>
auto Result<T, E, Exception>::or_else(F&& func) &&
{
    using Ret = std::invoke_result_t<F, E>;
    if (!_inner.has_value())
        return std::invoke(std::forward<F>(func), std::move(_inner).error());
    return Ret{Ok<T>{std::move(*_inner)}};
}

/**
 * @brief Or else / transform the Result to run a function on the error (if Err)
 * or return an Ok (if Ok).
 *
 * @tparam E
 * @tparam F
 * @param func
 * @return auto
 */
template <typename E, typename Exception>
template <std::invocable<E> F>
requires ResultLike<std::invoke_result_t<F, E>> &&
         std::is_void_v<typename std::invoke_result_t<F, E>::value_type>
auto Result<void, E, Exception>::or_else(F&& func) &&
{
    using Ret = std::invoke_result_t<F, E>;
    if (!_inner.has_value())
        return std::invoke(std::forward<F>(func), std::move(_inner).error());
    return Ret{ok()};
}

/**
 * @brief Flatten the Result to get the inner Result (if Ok) or return an Err
 * (if Err).
 *
 * @tparam T
 * @tparam E
 * @return auto
 */
template <typename T, typename E, typename Exception>
auto Result<T, E, Exception>::flatten() &&
requires ResultLike<T> && std::same_as<typename T::error_type, E>
{
    if (_inner.has_value())
        return std::move(*_inner);
    return T{Err<E>{std::move(_inner).error()}};
}

#endif   // __UTILS__INCLUDE__UTILS__RESULT__RESULT_TPP__