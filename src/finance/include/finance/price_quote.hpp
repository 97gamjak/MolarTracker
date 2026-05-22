#ifndef __FINANCE__INCLUDE__FINANCE__PRICE_QUOTE_HPP__
#define __FINANCE__INCLUDE__FINANCE__PRICE_QUOTE_HPP__

#include <expected>
#include <nlohmann/json.hpp>

#include "finance/cash.hpp"
#include "finance/finance_error.hpp"
#include "utils/timestamp.hpp"

namespace finance
{
    /**
     * @brief Represents a price quote for a financial instrument.
     *
     */
    struct PriceQuote
    {
        /// The price of the financial instrument.
        finance::Cash price;

        /// The timestamp of the price quote.
        Timestamp timestamp;

       public:
        PriceQuote(finance::Cash price_, Timestamp timestamp_);

        [[nodiscard]]
        static std::expected<PriceQuote, FinanceError> fromJson(
            const nlohmann::json& json
        );
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__PRICE_QUOTE_HPP__