#ifndef __FILTER__INCLUDE__FILTER__OPERATORS_HPP__
#define __FILTER__INCLUDE__FILTER__OPERATORS_HPP__

#include <cstdint>
#include <mstd/enum.hpp>

namespace filter
{
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define COMPARE_OPERATOR_LIST(X) \
    X(Equal)                     \
    X(NotEqual)                  \
    X(Less)                      \
    X(LessOrEqual)               \
    X(Greater)                   \
    X(GreaterOrEqual)            \
    X(In)                        \
    X(NotIn)                     \
    X(Between)

    MSTD_ENUM(Operator, std::uint8_t, COMPARE_OPERATOR_LIST);
}   // namespace filter

#endif   // __FILTER__INCLUDE__FILTER__OPERATORS_HPP__