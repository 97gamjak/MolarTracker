#include "utils/percentage.hpp"

#include <cmath>
#include <format>

/**
 * @brief Construct a new Percentage:: Percentage object
 *
 * @param value The initial value of the percentage.
 */
Percentage::Percentage(double value) : _value(value) {}

/**
 * @brief Returns a string representation of the percentage, formatted with two
 * decimal places and a percent sign, with a "+" sign for positive values.
 *
 * @param nDecimals The number of decimal places to include in the formatted
 * percentage string (default is 2).
 * @return std::string The formatted percentage string.
 */
std::string Percentage::toString(size_t nDecimals) const
{
    if (std::isnan(_value))
        return "-";

    return std::format("+{:.{}f}%", _value * _base, nDecimals);
}