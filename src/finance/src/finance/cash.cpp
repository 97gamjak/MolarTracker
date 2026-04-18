#include "finance/cash.hpp"

#include <cmath>
#include <format>
#include <string>

#include "currency_exception.hpp"
#include "finance/currency.hpp"

namespace finance
{

    /**
     * @brief Construct a new Cash:: Cash object
     *
     * @param currency
     */
    Cash::Cash(Currency currency) : _currency(currency), _amount(0) {}

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
     * @brief Equality operator for Cash
     *
     * @param lhs
     * @param rhs
     * @return true if both Cash objects have the same currency and amount
     * @return false otherwise
     */
    constexpr bool operator==(const Cash& lhs, const Cash& rhs)
    {
        if (lhs._currency != rhs._currency)
        {
            throw CurrencyMismatchException(
                "Cannot compare Cash objects with different currencies"
            );
        }

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
    Cash operator+(const Cash& lhs, const Cash& rhs)
    {
        if (lhs._currency != rhs._currency)
        {
            throw CurrencyMismatchException(
                "Cannot add Cash objects with different currencies"
            );
        }

        return Cash(lhs._currency, lhs._amount + rhs._amount);
    }

    /**
     * @brief Subtraction operator for Cash
     *
     * @param lhs
     * @param rhs
     * @return Cash the result of subtracting two Cash objects
     */
    Cash operator-(const Cash& lhs, const Cash& rhs)
    {
        if (lhs._currency != rhs._currency)
        {
            throw CurrencyMismatchException(
                "Cannot subtract Cash objects with different currencies"
            );
        }

        return Cash(lhs._currency, lhs._amount - rhs._amount);
    }

    /**
     * @brief Unary negation operator for Cash
     *
     * @param cash
     * @return Cash the result of negating a Cash object
     */
    Cash operator-(const Cash& cash)
    {
        return Cash(cash._currency, -cash._amount);
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
        if (lhs._currency != rhs._currency)
        {
            throw CurrencyMismatchException(
                "Cannot add Cash objects with different currencies"
            );
        }

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
        if (lhs._currency != rhs._currency)
        {
            throw CurrencyMismatchException(
                "Cannot subtract Cash objects with different currencies"
            );
        }

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
     * @return std::string The string representation of the Cash object.
     */
    std::string Cash::toString() const { return toString(std::nullopt); }

    /**
     * @brief Converts the Cash object to a string representation.
     *
     * @param nDecimalPlaces An optional parameter specifying the number of
     * decimal places to include in the string representation. If not provided,
     * it will default to the number of decimal places defined from the
     * currency's micro unit.
     *
     * @return std::string The string representation of the Cash object.
     */
    std::string Cash::toString(std::optional<std::uint8_t> nDecimalPlaces) const
    {
        const auto amount        = getAmount();
        const auto microUnit     = getMicroUnit(getCurrency());
        const auto decimalPlaces = nDecimalPlaces.value_or(microUnit);

        const auto scale = static_cast<int64_t>(std::pow(10, microUnit));

        const auto intPart  = std::abs(amount / scale);
        const auto fracPart = std::abs(amount % scale);
        const char sign     = amount < 0 ? '-' : '\0';

        const std::string decimal = std::format(
            "{}{}.{:0{}d}",   // pad fracPart to _nDecimalPlaces digits
            (sign != 0) ? std::string(1, sign) : "",
            intPart,
            fracPart,
            decimalPlaces
        );

        return std::format("{} {}", decimal, getSymbol(getCurrency()));
    }

}   // namespace finance