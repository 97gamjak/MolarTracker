#ifndef __UTILS__INCLUDE__UTILS__QUANTITY_TPP__
#define __UTILS__INCLUDE__UTILS__QUANTITY_TPP__

#include "quantity.hpp"

/**
 * @brief Compare this quantity to a scalar value for greater-than.
 *
 * @tparam T The type of the scalar value.
 * @param lhs The quantity to compare.
 * @param rhs The scalar value to compare against.
 * @return true if the quantity is greater than the scalar value, false
 * otherwise.
 */
template <typename T>
[[nodiscard]] bool operator>(const Quantity& lhs, const T& rhs)
{
    return lhs.getValue() > rhs;
}

#endif   // __UTILS__INCLUDE__UTILS__QUANTITY_TPP__