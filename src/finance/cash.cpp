#include "cash.hpp"

#include "currency_exception.hpp"

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

}   // namespace finance