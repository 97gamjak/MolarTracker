#include "utils/cash.hpp"

#include <cmath>
#include <format>
#include <string>

#include "currency_exception.hpp"
#include "utils/currency.hpp"
#include "utils/quantity.hpp"

/**
 * @brief Construct a new Cash:: Cash object with a specified amount
 *
 * @param currency
 * @param amount
 */
Cash::Cash(Currency currency, micro_units amount)
    : _currency(currency), _amount(amount)
{
}

/**
 * @brief Construct a new Cash:: Cash object
 *
 */
Cash::Cash() : _currency(Currency::Unknown), _amount(0) {}

/**
 * @brief Equality operator for Cash
 *
 * @param lhs
 * @param rhs
 * @return true if both Cash objects have the same currency and amount
 * @return false otherwise
 */
bool operator==(Cash lhs, const Cash& rhs)
{
    lhs._takeCurrency(rhs);
    return lhs._amount == rhs._amount;
}

/**
 * @brief Three-way comparison operator for Cash
 *
 * @param lhs
 * @param rhs
 * @return std::strong_ordering the result of the comparison
 */
constexpr std::strong_ordering operator<=>(const Cash& lhs, const Cash& rhs)
{
    if (lhs._currency != rhs._currency)
    {
        throw CurrencyMismatchException(
            "Cannot compare Cash objects with different currencies"
        );
    }

    return lhs._amount <=> rhs._amount;
}

/**
 * @brief Addition operator for Cash
 *
 * @param lhs
 * @param rhs
 * @return Cash the result of adding two Cash objects
 */
Cash operator+(Cash lhs, const Cash& rhs)
{
    lhs._takeCurrency(rhs);

    return Cash{lhs._currency, lhs._amount + rhs._amount};
}

/**
 * @brief Subtraction operator for Cash
 *
 * @param lhs
 * @param rhs
 * @return Cash the result of subtracting two Cash objects
 */
Cash operator-(Cash lhs, const Cash& rhs) { return lhs + (-rhs); }

/**
 * @brief Unary negation operator for Cash
 *
 * @param cash
 * @return Cash the result of negating a Cash object
 */
Cash operator-(const Cash& cash) { return Cash{cash._currency, -cash._amount}; }

/**
 * @brief Multiplication operator for Cash
 *
 * @param cash
 * @param multiplier
 * @return Cash the result of multiplying a Cash object by a Quantity
 */
Cash operator*(const Cash& cash, const Quantity& multiplier)
{
    return Cash{cash._currency, mulDiv(cash._amount, multiplier)};
}

/**
 * @brief Multiplication operator for Cash
 *
 * @param multiplier
 * @param cash
 * @return Cash the result of multiplying a Quantity by a Cash object
 */
Cash operator*(const Quantity& multiplier, const Cash& cash)
{
    return cash * multiplier;
}

/**
 * @brief Division operator for Cash
 *
 * @param cash
 * @param divisor
 * @return Cash
 */
Cash operator/(const Cash& cash, const Quantity& divisor)
{
    return Cash{cash._currency, divBy(cash._amount, divisor)};
}

/**
 * @brief Division operator for Cash
 *
 * @param cash
 * @param divisor
 * @return double
 */
double operator/(const Cash& cash, const Cash& divisor)
{
    return static_cast<double>(cash._amount) /
           static_cast<double>(divisor._amount);
}

/**
 * @brief Compound addition assignment operator for Cash
 *
 * @param lhs
 * @param rhs
 * @return Cash& the result of adding two Cash objects and assigning the
 * result to the left-hand side object
 */
Cash& operator+=(Cash& lhs, const Cash& rhs)
{
    lhs._takeCurrency(rhs);

    lhs._amount += rhs._amount;
    return lhs;
}

/**
 * @brief Compound subtraction assignment operator for Cash
 *
 * @param lhs
 * @param rhs
 * @return Cash& the result of subtracting two Cash objects and assigning
 * the result to the left-hand side object
 */
Cash& operator-=(Cash& lhs, const Cash& rhs)
{
    lhs._takeCurrency(rhs);

    lhs._amount -= rhs._amount;
    return lhs;
}

/**
 * @brief Checks if the Cash amount is zero.
 *
 * @return true if the amount is zero, false otherwise.
 */
bool Cash::isZero() const { return _amount == 0; }

/**
 * @brief Checks if the Cash amount is positive.
 *
 * @return true if the amount is greater than zero, false otherwise.
 */
bool Cash::isPositive() const { return _amount > 0; }

/**
 * @brief Checks if the Cash amount is negative.
 *
 * @return true if the amount is less than zero, false otherwise.
 */
bool Cash::isNegative() const { return _amount < 0; }

/**
 * @brief Gets the amount of cash in micro_units.
 *
 * @return micro_units The amount of cash.
 */
micro_units Cash::getAmount() const { return _amount; }

/**
 * @brief Gets the currency of the cash.
 *
 * @return Currency The currency of the cash.
 */
Currency Cash::getCurrency() const { return _currency; }

/**
 * @brief Converts the Cash object to a string representation.
 *
 * @param nDecimalPlaces An optional parameter specifying the number of
 * decimal places to include in the string representation. If not provided,
 * it will default to the number of decimal places defined from the
 * currency's micro unit.
 * @param includeCurrencySymbol A boolean flag indicating whether to include
 * the currency symbol in the string representation. Defaults to false.
 * @param includeDecimalPoint A boolean flag indicating whether to include the
 * decimal point in the string representation, even if the number of
 * decimal places is zero. Defaults to false.
 *
 * @return std::string The string representation of the Cash object.
 */
std::string Cash::toString(
    std::optional<std::uint8_t> nDecimalPlaces,
    bool                        includeCurrencySymbol,
    bool                        includeDecimalPoint
) const
{
    const auto amount        = getAmount();
    const auto microUnit     = getMicroUnit(getCurrency());
    const auto decimalPlaces = nDecimalPlaces.value_or(microUnit);

    auto result = microUnitsToString(
        amount,
        decimalPlaces,
        microUnit,
        includeDecimalPoint
    );

    if (!includeCurrencySymbol || getCurrency() == Currency::Unknown)
        return result;

    return std::format("{} {}", result, getSymbol(getCurrency()));
}

/**
 * @brief Handle remapping of position IDs in transaction entries
 *
 * @param cash
 */
void Cash::_takeCurrency(const Cash& cash)
{
    if (cash._currency == Currency::Unknown)
        return;   // rhs is currency-less, nothing to reconcile

    if (_currency == Currency::Unknown)
    {
        _currency = cash._currency;
    }
    else if (_currency != cash._currency)
    {
        throw CurrencyMismatchException(
            std::format(
                "Cannot operate on Cash objects with different currencies: "
                "{} vs {}",
                CurrencyMeta::toString(_currency),
                CurrencyMeta::toString(cash._currency)
            )
        );
    }
}

/**
 * @brief Returns the maximum of two Cash objects, ensuring they have the same
 * currency.
 *
 * @param lhs The first Cash object.
 * @param rhs The second Cash object.
 * @return Cash The Cash object with the greater amount.
 */
Cash Cash::max(const Cash& lhs, const Cash& rhs)
{
    if (lhs._currency != rhs._currency)
    {
        throw CurrencyMismatchException(
            "Cannot compare Cash objects with different currencies"
        );
    }

    return lhs._amount >= rhs._amount ? lhs : rhs;
}

/**
 * @brief Returns the minimum of two Cash objects, ensuring they have the same
 * currency.
 *
 * @param lhs The first Cash object.
 * @param rhs The second Cash object.
 * @return Cash The Cash object with the lesser amount.
 */
Cash Cash::min(const Cash& lhs, const Cash& rhs)
{
    if (lhs._currency != rhs._currency)
    {
        throw CurrencyMismatchException(
            "Cannot compare Cash objects with different currencies"
        );
    }

    return lhs._amount <= rhs._amount ? lhs : rhs;
}