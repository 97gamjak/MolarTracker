#include "config/quantity.hpp"

#include <format>

#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#endif

/**
 * @brief Construct a new Quantity:: Quantity object
 *
 * @param value
 */
Quantity::Quantity(micro_units value) : _value(value) {}

/**
 * @brief get the value of the quantity as a double
 *
 * @return double
 */
double Quantity::getValue() const
{
    return static_cast<double>(_value) / static_cast<double>(factor);
}

/**
 * @brief get the raw value of the quantity in micro_units
 *
 * @return micro_units
 */
micro_units Quantity::toMicroUnits() const { return _value; }

/**
 * @brief get the string representation of the quantity
 *
 * @return std::string
 */
std::string Quantity::toString() const
{
    return std::format("{:.{}f}", getValue(), precision);
}

/**
 * @brief Compare this quantity to another quantity for greater-than.
 *
 * @param other The other quantity to compare against.
 * @return true if this quantity is greater than the other, false otherwise.
 */
bool Quantity::operator>(const Quantity& other) const
{
    return _value > other._value;
}

/**
 * @brief Negate the quantity.
 *
 * @param quantity The quantity to negate.
 * @return A new quantity representing the negated value.
 */
[[nodiscard]] Quantity operator-(const Quantity& quantity)
{
    return Quantity(-quantity._value);
}

/**
 * @brief Multiply two micro_units values and divide by a third.
 *
 * @param lhs The left-hand side value.
 * @param rhs The right-hand side value.
 * @param divisor The divisor.
 * @return The result of the multiplication and division.
 */
[[nodiscard]] micro_units mulDiv(
    micro_units lhs,
    micro_units rhs,
    micro_units divisor
)
{
#if defined(_MSC_VER) && !defined(__clang__)
    int64_t    high;
    const auto low = static_cast<uint64_t>(_mul128(a, b, &high));
    int64_t    remainder;
    return _div128(high, low, divisor, &remainder);
#else
    // clang-format off
    _Pragma("GCC diagnostic push")
    _Pragma("GCC diagnostic ignored \"-Wpedantic\"") 
    const auto result =
        static_cast<micro_units>(static_cast<__int128>(lhs) * rhs / divisor);
    _Pragma("GCC diagnostic pop")
    return result;
    // clang-format on
#endif
}

/**
 * @brief Multiply two a micro_unit with a Quantity and divide by the Quantity's
 * factor.
 *
 * @param lhs The left-hand side micro_unit value.
 * @param rhs The right-hand side Quantity value.
 * @return The result of the multiplication and division.
 */
[[nodiscard]] micro_units mulDiv(micro_units lhs, Quantity rhs)
{
    return mulDiv(lhs, rhs.toMicroUnits(), Quantity::factor);
}