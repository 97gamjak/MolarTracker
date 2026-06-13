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
    template <typename E>
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

/**
 * @brief Construct a new Result< T,  E>:: Result object
 *
 * @tparam T
 * @tparam E
 * @param object
 */
template <typename T, typename E>
Result<T, E>::Result(Ok<T> object) : _inner(std::move(object.value))
{
}

/**
 * @brief Construct a new Result< T,  E>:: Result object
 *
 * @tparam T
 * @tparam E
 * @param object
 */
template <typename T, typename E>
Result<T, E>::Result(Err<E> object)
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
template <typename T, typename E>
Result<T, E>::Result(std::expected<T, E> exp) : _inner(std::move(exp))
{
}

/**
 * @brief Construct a new Result<void, E>::Result object
 *
 * @tparam E
 * @param ok
 */
template <typename E>
Result<void, E>::Result(Ok<void> /*ok*/) : _inner()
{
}

/**
 * @brief Construct a new Result<void, E>::Result object
 *
 * @tparam E
 * @param error
 */
template <typename E>
Result<void, E>::Result(Err<E> error)
    : _inner(std::unexpected(std::move(error.error)))
{
}

/**
 * @brief Construct a new Result<void, E>::Result object
 *
 * @tparam E
 * @param exp
 */
template <typename E>
Result<void, E>::Result(std::expected<void, E> exp) : _inner(std::move(exp))
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
template <typename T, typename E>
bool Result<T, E>::has_value() const
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
template <typename E>
bool Result<void, E>::has_value() const
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
template <typename T, typename E>
Result<T, E>::operator bool() const
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
template <typename E>
Result<void, E>::operator bool() const
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
template <typename T, typename E>
T& Result<T, E>::operator*() &
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
template <typename T, typename E>
const T& Result<T, E>::operator*() const&
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
template <typename T, typename E>
T&& Result<T, E>::operator*() &&
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
template <typename T, typename E>
T* Result<T, E>::operator->()
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
template <typename T, typename E>
const T* Result<T, E>::operator->() const
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
template <typename T, typename E>
T& Result<T, E>::value() &
{
    return _inner.value();
}

/**
 * @brief Get the value (if Ok) or throw (if Err).
 *
 * @tparam E
 * @return const T&
 */
template <typename E>
void Result<void, E>::value() const
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
template <typename T, typename E>
const T& Result<T, E>::value() const&
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
template <typename T, typename E>
T&& Result<T, E>::value() &&
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
template <typename T, typename E>
E& Result<T, E>::error() &
{
    return _inner.error();
}

/**
 * @brief Get the error (if Err) or throw (if Ok).
 *
 * @tparam E
 * @return const E&
 */
template <typename E>
E& Result<void, E>::error() &
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
template <typename T, typename E>
const E& Result<T, E>::error() const&
{
    return _inner.error();
}

/**
 * @brief Get the error (if Err) or throw (if Ok).
 *
 * @tparam E
 * @return const E&
 */
template <typename E>
const E& Result<void, E>::error() const&
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
template <typename T, typename E>
E&& Result<T, E>::error() &&
{
    return std::move(_inner).error();
}

/**
 * @brief Get the error (if Err) or throw (if Ok).
 *
 * @tparam E
 * @return E&&
 */
template <typename E>
E&& Result<void, E>::error() &&
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
template <typename T, typename E>
T Result<T, E>::unwrap() &&
{
    if (!_inner.has_value())
        throw std::runtime_error(detail::format_error(_inner.error()));
    return std::move(*_inner);
}

/**
 * @brief Unwrap the Result to get the value (if Ok) or throw (if Err).
 *
 * @tparam E
 */
template <typename E>
void Result<void, E>::unwrap() &&
{
    if (!_inner.has_value())
        throw std::runtime_error(detail::format_error(_inner.error()));
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
template <typename T, typename E>
T Result<T, E>::unwrap_or(T fallback) &&
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
template <typename T, typename E>
template <std::invocable<const E&> F>
requires std::convertible_to<std::invoke_result_t<F, const E&>, T>
T Result<T, E>::unwrap_or_else(F&& func) &&
{
    if (_inner.has_value())
        return std::move(*_inner);
    return static_cast<T>(std::invoke(std::forward<F>(func), _inner.error()));
}

/**
 * @brief Inspect the Result to run a function on the value (if Ok) or do
 * nothing (if Err).
 *
 * @tparam T
 * @tparam E
 * @tparam F
 * @param func
 * @return Result<T, E>&
 */
template <typename T, typename E>
template <std::invocable<const T&> F>
Result<T, E>& Result<T, E>::inspect(F&& func) &
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
 * @return Result<void, E>&
 */
template <typename E>
template <std::invocable F>
Result<void, E>& Result<void, E>::inspect(F&& func) &
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
 * @return Result<T, E>&&
 */
template <typename T, typename E>
template <std::invocable<const T&> F>
Result<T, E>&& Result<T, E>::inspect(F&& func) &&
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
 * @return Result<void, E>&&
 */
template <typename E>
template <std::invocable F>
Result<void, E>&& Result<void, E>::inspect(F&& func) &&
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
 * @return Result<T, E>&
 */
template <typename T, typename E>
template <std::invocable<const E&> F>
Result<T, E>& Result<T, E>::inspect_error(F&& func) &
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
 * @return Result<void, E>&
 */
template <typename E>
template <std::invocable<const E&> F>
Result<void, E>& Result<void, E>::inspect_error(F&& func) &
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
 * @return Result<T, E>&&
 */
template <typename T, typename E>
template <std::invocable<const E&> F>
Result<T, E>&& Result<T, E>::inspect_error(F&& func) &&
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
 * @return Result<void, E>&&
 */
template <typename E>
template <std::invocable<const E&> F>
Result<void, E>&& Result<void, E>::inspect_error(F&& func) &&
{
    if (!_inner.has_value())
        std::invoke(std::forward<F>(func), _inner.error());
    return std::move(*this);
}

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
template <typename T, typename E>
template <std::invocable<T> F>
requires ResultLike<std::invoke_result_t<F, T>> &&
         std::same_as<typename std::invoke_result_t<F, T>::error_type, E>
auto Result<T, E>::and_then(F&& func) &&
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
template <typename E>
template <std::invocable F>
auto Result<void, E>::then(F&& func) &&
{
    using U = std::invoke_result_t<F>;
    if constexpr (std::is_void_v<U>)
    {
        if (_inner.has_value())
        {
            std::invoke(std::forward<F>(func));
            return Result<void, E>{ok()};
        }
        return Result<void, E>{Err<E>{std::move(_inner).error()}};
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
template <typename E>
template <std::invocable F>
requires ResultLike<std::invoke_result_t<F>> &&
         std::same_as<typename std::invoke_result_t<F>::error_type, E>
auto Result<void, E>::and_then(F&& func) &&
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
template <typename T, typename E>
template <std::invocable<T> F>
auto Result<T, E>::transform(F&& func) &&
{
    using U = std::invoke_result_t<F, T>;
    if constexpr (std::is_void_v<U>)
    {
        if (_inner.has_value())
        {
            std::invoke(std::forward<F>(func), std::move(*_inner));
            return Result<void, E>{ok()};
        }
        return Result<void, E>{Err<E>{std::move(_inner).error()}};
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
template <typename E>
template <std::invocable F>
auto Result<void, E>::transform(F&& func) &&
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
template <typename T, typename E>
template <std::invocable<T> F>
auto Result<T, E>::map(F&& func) &&
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
template <typename T, typename E>
template <std::invocable<E> F>
auto Result<T, E>::transform_error(F&& func) &&
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
template <typename E>
template <std::invocable<E> F>
auto Result<void, E>::transform_error(F&& func) &&
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
template <typename T, typename E>
template <std::invocable<E> F>
auto Result<T, E>::map_error(F&& func) &&
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
template <typename E>
template <std::invocable<E> F>
auto Result<void, E>::map_error(F&& func) &&
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
template <typename T, typename E>
template <std::invocable<E> F>
requires ResultLike<std::invoke_result_t<F, E>> &&
         std::same_as<typename std::invoke_result_t<F, E>::value_type, T>
auto Result<T, E>::or_else(F&& func) &&
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
template <typename E>
template <std::invocable<E> F>
requires ResultLike<std::invoke_result_t<F, E>> &&
         std::is_void_v<typename std::invoke_result_t<F, E>::value_type>
auto Result<void, E>::or_else(F&& func) &&
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
template <typename T, typename E>
auto Result<T, E>::flatten() &&
requires ResultLike<T> && std::same_as<typename T::error_type, E>
{
    if (_inner.has_value())
        return std::move(*_inner);
    return T{Err<E>{std::move(_inner).error()}};
}

#endif   // __UTILS__INCLUDE__UTILS__RESULT__RESULT_TPP__