#ifndef __FINANCE__INCLUDE__FINANCE__PRICE_QUOTE_HPP__
#define __FINANCE__INCLUDE__FINANCE__PRICE_QUOTE_HPP__

#include <expected>
#include <nlohmann/json.hpp>

#include "finance/finance_error.hpp"
#include "utils/cash.hpp"
#include "utils/timestamp.hpp"

namespace finance
{
    /**
     * @brief Represents a price quote for a financial instrument.
     *
     */
    class PriceQuote
    {
       private:
        /// The price of the financial instrument.
        Cash _price;

        /// The timestamp of the price quote.
        Timestamp _timestamp;

       public:
        PriceQuote(Cash price, Timestamp timestamp);

        [[nodiscard]]
        static std::expected<PriceQuote, FinanceError> fromJson(
            const nlohmann::json& json
        );

        [[nodiscard]]
        const Cash& getPrice() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__PRICE_QUOTE_HPP__