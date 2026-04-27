#ifndef __FINANCE__INCLUDE__FINANCE__SECURITY_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__SECURITY_TRANSACTION_HPP__

#include <string>

#include "config/finance.hpp"

namespace finance
{
    /**
     * @brief Represents a security transaction.
     *
     */
    class StockTransaction
    {
       private:
        std::string _ticker;

        Currency _currency;

        micro_units _quantity;

       public:
        explicit StockTransaction(
            std::string ticker,
            Currency    currency,
            micro_units quantity
        );

        [[nodiscard]] const std::string& getTicker() const;
        [[nodiscard]] Currency           getCurrency() const;
        [[nodiscard]] micro_units        getQuantity() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__SECURITY_TRANSACTION_HPP__
