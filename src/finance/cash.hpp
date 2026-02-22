#ifndef __FINANCE__CASH_HPP__
#define __FINANCE__CASH_HPP__

#include <compare>

#include "currency.hpp"

namespace finance
{
    /// Type alias for micro units (e.g., millionths of a currency unit)
    using micro_units = std::int64_t;

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
        explicit Cash(Currency currency);
        explicit Cash(Currency currency, micro_units amount);

        friend constexpr bool operator==(const Cash& lhs, const Cash& rhs);
        friend constexpr std::strong_ordering operator<=>(
            const Cash& lhs,
            const Cash& rhs
        );

        friend Cash operator+(const Cash& lhs, const Cash& rhs);
        friend Cash operator-(const Cash& lhs, const Cash& rhs);
        friend Cash operator-(const Cash& cash);
    };

}   // namespace finance

#endif   // __FINANCE__CASH_HPP__