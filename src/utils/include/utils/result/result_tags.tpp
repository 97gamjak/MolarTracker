#ifndef __UTILS__INCLUDE__UTILS__RESULT__RESULT_TAGS_TPP__
#define __UTILS__INCLUDE__UTILS__RESULT__RESULT_TAGS_TPP__

#include <utility>

#include "result_tags.hpp"

/**
 * @brief Construct a new Ok<T>::Ok object
 *
 * @tparam T
 * @param value_
 */
template <typename T>
Ok<T>::Ok(T value_) : value(std::move(value_))
{
}

/**
 * @brief Construct a new Err<E>::Err object
 *
 * @tparam E
 * @param error_
 */
template <typename E>
Err<E>::Err(E error_) : error(std::move(error_))
{
}

#endif   // __UTILS__INCLUDE__UTILS__RESULT__RESULT_TAGS_TPP__