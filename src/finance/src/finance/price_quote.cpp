#include "finance/price_quote.hpp"

#include <expected>

#include "config/finance.hpp"
#include "finance/currency.hpp"
#include "finance/finance_error.hpp"
#include "json/json.hpp"

namespace finance
{
    /**
     * @brief Constructs a PriceQuote object.
     *
     * @param price The price of the financial instrument.
     * @param timestamp The timestamp of the price quote.
     */
    PriceQuote::PriceQuote(finance::Cash price, Timestamp timestamp)
        : _price(price), _timestamp(timestamp)
    {
    }

    /**
     * @brief Create a PriceQuote object from JSON.
     *
     * @param json The JSON object containing the price quote data.
     * @return std::expected<PriceQuote, FinanceError>
     */
    std::expected<PriceQuote, FinanceError> PriceQuote::fromJson(
        const nlohmann::json& json
    )
    {
        const auto& data =
            json.at("quoteSummary").at("result").at(0).at("price");

        const auto currencyStr = json::_safeGet<std::string>(data, "currency");
        const auto currencyOpt = CurrencyMeta::from_string(currencyStr);

        if (!currencyOpt)
        {
            return std::unexpected(FinanceError(
                FinanceErrorType::CurrencyUnknown,
                "Unknown currency " + currencyStr
            ));
        }
        const auto currency = currencyOpt.value();

        const auto priceStr =
            json::_safeGet<std::string>(data, "regularMarketPreviousClose");
        const auto price =
            microUnitsFromString(priceStr, getMicroUnit(currency));

        const auto time = json::_safeGet<int64_t>(data, "regularMarketTime");
        const auto timeStamp = Timestamp::fromInt64(time);

        return PriceQuote{
            Cash{currency, price},
            timeStamp,
        };
    }

}   // namespace finance
