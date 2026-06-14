#ifndef __UTILS__INCLUDE__UTILS__RESULT__RESULT_TAGS_HPP__
#define __UTILS__INCLUDE__UTILS__RESULT__RESULT_TAGS_HPP__

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

    explicit Ok(T value_);
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

    explicit Err(E error_);

    template <typename... Args>
    explicit Err(Args&&... args);
};

template <typename E>
Err(E) -> Err<E>;

/**
 * @brief Factory for Result<void, E> success paths.
 */
[[nodiscard]] inline Ok<void> ok() { return {}; }

#ifndef __UTILS__INCLUDE__UTILS__RESULT__RESULT_TAGS_TPP__
#include "result_tags.tpp"
#endif   // __UTILS__INCLUDE__UTILS__RESULT__RESULT_TAGS_TPP__

#endif   // __UTILS__INCLUDE__UTILS__RESULT__RESULT_TAGS_HPP__