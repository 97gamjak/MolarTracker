#include "utils/quantity.hpp"

#include <charconv>
#include <cmath>
#include <format>
#include <stdexcept>

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
 * @brief Check if the quantity is zero.
 *
 * @return true if the quantity is zero, false otherwise.
 */
bool Quantity::isZero() const { return _value == 0; }

/**
 * @brief Get the absolute value of the quantity.
 *
 * @return A new Quantity object representing the absolute value of this
 * quantity.
 */
Quantity Quantity::abs() const
{
    return Quantity(_value < 0 ? -_value : _value);
}

/**
 * @brief Compare this quantity to another quantity for equality.
 *
 * @param other The other quantity to compare against.
 * @return true if the quantities are equal, false otherwise.
 */
bool Quantity::operator==(const Quantity& other) const
{
    return _value == other._value;
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
 * @brief Adds another quantity to this quantity.
 *
 * @param other The other quantity to add.
 * @return A reference to this quantity.
 */
Quantity& Quantity::operator+=(const Quantity& other)
{
    _value += other._value;
    return *this;
}

/**
 * @brief Subtracts another quantity from this quantity.
 *
 * @param other The other quantity to subtract.
 * @return A reference to this quantity.
 */
Quantity& Quantity::operator-=(const Quantity& other)
{
    _value -= other._value;
    return *this;
}

/**
 * @brief Adds two quantities together.
 *
 * @param lhs The left-hand side quantity.
 * @param rhs The right-hand side quantity.
 * @return A new Quantity object representing the sum of the two quantities.
 */
Quantity operator+(const Quantity& lhs, const Quantity& rhs)
{
    return Quantity(lhs._value + rhs._value);
}

/**
 * @brief Subtracts one quantity from another.
 *
 * @param lhs The left-hand side quantity.
 * @param rhs The right-hand side quantity.
 * @return A new Quantity object representing the difference between the two
 * quantities.
 */
Quantity operator-(const Quantity& lhs, const Quantity& rhs)
{
    return Quantity(lhs._value - rhs._value);
}

/**
 * @brief Negate the quantity.
 *
 * @param quantity The quantity to negate.
 * @return A new quantity representing the negated value.
 */
Quantity operator-(const Quantity& quantity)
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
micro_units mulDiv(micro_units lhs, micro_units rhs, micro_units divisor)
{
#if defined(_MSC_VER) && !defined(__clang__)
    int64_t    high;
    const auto low = static_cast<uint64_t>(_mul128(lhs, rhs, &high));
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
micro_units mulDiv(micro_units lhs, Quantity rhs)
{
    return mulDiv(lhs, rhs.toMicroUnits(), Quantity::factor);
}

/**
 * @brief Divide a micro_unit by a Quantity.
 *
 * @param lhs The left-hand side micro_unit value.
 * @param rhs The right-hand side Quantity value.
 * @return The result of the division.
 */
micro_units divBy(micro_units lhs, Quantity rhs)
{
    if (rhs.isZero())
        throw std::domain_error("Division by zero");

    return mulDiv(
        lhs,
        static_cast<micro_units>(Quantity::factor),
        rhs.toMicroUnits()
    );
}

/**
 * @brief Convert a string representation of a quantity to micro_units.
 *
 * @param value The string representation of the quantity.
 * @param precision The number of decimal places to consider.
 * @return The quantity in micro_units.
 */
micro_units microUnitsFromString(std::string_view value, std::uint8_t precision)
{
    if (value.empty())
        throw std::invalid_argument("microUnitsFromString: empty string");

    const int sign = value.front() == '-' ? -1 : 1;
    if (sign < 0)
        value.remove_prefix(1);

    if (value.empty())
        throw std::invalid_argument("microUnitsFromString: bare '-'");

    const auto dotPos = value.find('.');

    const std::string_view intStr  = value.substr(0, dotPos);
    const std::string_view fracStr = dotPos != std::string_view::npos
                                         ? value.substr(dotPos + 1)
                                         : std::string_view{};

    // Compute scale = 10^precision in integer arithmetic.
    const auto base  = 10;
    int64_t    scale = 1;
    for (std::size_t i = 0; i < precision; ++i)
        scale *= base;

    // Parse integer part.
    // Parse integer part — std::from_chars detects out-of-range for us.
    int64_t intPart = 0;
    if (!intStr.empty())
    {
        const std::string_view view{intStr};
        // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        const auto [ptr, ec] =
            std::from_chars(view.data(), view.data() + view.size(), intPart);

        if (ec == std::errc::result_out_of_range)
            throw std::overflow_error(
                "microUnitsFromString: integer part overflows int64_t"
            );

        if (ec != std::errc{} || ptr != view.data() + view.size())
            throw std::invalid_argument(
                "microUnitsFromString: malformed integer part"
            );
        // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }

    // Parse fractional part: truncate or right-pad with zeros to exactly
    // `precision` digits — same policy as AmountLineEdit::getAmount.
    int64_t fracPart  = 0;
    int64_t fracScale = scale;
    for (std::size_t i = 0; i < precision; ++i)
    {
        fracScale            /= base;
        const char character  = i < fracStr.size() ? fracStr[i] : '0';

        if (character < '0' || character > '9')
        {
            throw std::invalid_argument(
                std::string{"microUnitsFromString: unexpected char '"} +
                character + "'"
            );
        }
        fracPart += (character - '0') * fracScale;
    }

    return sign * ((intPart * scale) + fracPart);
}

/**
 * @brief Convert a micro_units value to a string representation with the given
 * precision.
 *
 * @param value The micro_units value to convert.
 * @param precision The number of decimal places to include in the string
 * representation.
 * @param withDecimalPoint Whether to include the decimal point in the string
 * representation (default: true).
 * @return The string representation of the quantity.
 */
std::string microUnitsToString(
    micro_units  value,
    std::uint8_t precision,
    std::uint8_t scale,
    bool         withDecimalPoint
)
{
    const auto factor   = static_cast<int64_t>(std::pow(10, scale));
    const auto intPart  = std::abs(value / factor);
    const auto fracPart = std::abs(value % factor);
    const char sign     = value < 0 ? '-' : '\0';

    auto result = std::format(
        "{}{}.{:0{}d}",
        (sign != 0) ? std::string(1, sign) : "",
        intPart,
        fracPart,
        precision
    );

    if (!withDecimalPoint && precision > 0)
    {
        const auto dotPos = result.find('.');
        if (dotPos != std::string::npos)
            result.erase(dotPos, 1);
    }

    return result;
}