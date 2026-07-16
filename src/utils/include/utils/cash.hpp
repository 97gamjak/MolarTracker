#ifndef __UTILS__INCLUDE__UTILS__CASH_HPP__
#define __UTILS__INCLUDE__UTILS__CASH_HPP__

#include <compare>

#include "utils/finance.hpp"
#include "utils/quantity.hpp"

/**
 * @brief A class representing a cash amount in a specific currency.
 *
 * This class provides basic arithmetic operations and comparisons for cash
 * amounts. It ensures that operations are only performed between amounts of
 * the same currency.
 *
 */
class Cash
{
   private:
    /// The currency of the cash amount
    Currency _currency;
    /// The amount in micro units (e.g., millionths of a currency unit)
    micro_units _amount;

   public:
    Cash(Currency currency, micro_units amount);
    Cash();

    friend constexpr bool                 operator==(Cash lhs, const Cash& rhs);
    friend constexpr std::strong_ordering operator<=>(
        const Cash& lhs,
        const Cash& rhs
    );

    friend Cash   operator+(Cash lhs, const Cash& rhs);
    friend Cash   operator-(Cash lhs, const Cash& rhs);
    friend Cash   operator-(const Cash& cash);
    friend Cash   operator*(const Cash& cash, const Quantity& multiplier);
    friend Cash   operator*(const Quantity& multiplier, const Cash& cash);
    friend Cash   operator/(const Cash& cash, const Quantity& divisor);
    friend double operator/(const Cash& cash, const Cash& divisor);

    friend Cash& operator+=(Cash& lhs, const Cash& rhs);
    friend Cash& operator-=(Cash& lhs, const Cash& rhs);

    [[nodiscard]] bool isZero() const;
    [[nodiscard]] bool isPositive() const;
    [[nodiscard]] bool isNegative() const;

    [[nodiscard]] micro_units getAmount() const;
    [[nodiscard]] Currency    getCurrency() const;
    [[nodiscard]] std::string toString(
        std::optional<std::uint8_t> nDecimalPlaces        = std::nullopt,
        bool                        includeCurrencySymbol = true,
        bool                        includeDecimalPoint   = true
    ) const;

   private:
    // cppcheck-suppress unusedPrivateFunction -- used in friend operators
    void _takeCurrency(const Cash& cash);
};

#endif   // __UTILS__INCLUDE__UTILS__CASH_HPP__