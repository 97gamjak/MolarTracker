#ifndef __FINANCE__INCLUDE__FINANCE__PRICE_QUOTE_HPP__
#define __FINANCE__INCLUDE__FINANCE__PRICE_QUOTE_HPP__

#include <nlohmann/json.hpp>

#include "finance/cash.hpp"
#include "utils/result/error.hpp"
#include "utils/result/result.hpp"
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
        finance::Cash _price;

        /// The timestamp of the price quote.
        Timestamp _timestamp;

       public:
        PriceQuote(finance::Cash price, Timestamp timestamp);

        [[nodiscard]]
        static MTResult<PriceQuote> fromJson(const nlohmann::json& json);

        [[nodiscard]]
        const finance::Cash& getPrice() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__PRICE_QUOTE_HPP__