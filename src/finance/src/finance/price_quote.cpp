#include "finance/price_quote.hpp"

#include <stdexcept>

#include "config/finance.hpp"
#include "finance/currency.hpp"
#include "json/json.hpp"
#include "logging/log_macros.hpp"
#include "utils/result/error.hpp"

REGISTER_LOG_CATEGORY("Finance.PriceQuote");

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
     * @return MTResult<PriceQuote>
     */
    MTResult<PriceQuote> PriceQuote::fromJson(const nlohmann::json& json)
    {
        const auto& data =
            json.at("quoteSummary").at("result").at(0).at("price");

        LOG_TRACE(std::format("Parsing price quote JSON: {}", data.dump()));

        const auto currencyStr = json::safeGet<std::string>(data, "currency");
        const auto currencyOpt = CurrencyMeta::from_string(currencyStr);

        if (!currencyOpt)
        {
            return Err<Error>{
                err::FinanceError::CurrencyUnknown,
                "Unknown currency " + currencyStr
            };
        }
        const auto currency = currencyOpt.value();

        const auto priceStr = std::to_string(
            json::safeGet<double>(data.at("regularMarketPreviousClose"), "raw")
        );

        micro_units price = 0;

        try
        {
            price = microUnitsFromString(priceStr, getMicroUnit(currency));
        }
        catch (const std::overflow_error& e)
        {
            return Err<Error>{
                err::FinanceError::PriceOverflow,
                "Price overflow for " + priceStr + ": " + e.what()
            };
        }
        catch (const std::invalid_argument& e)
        {
            return Err<Error>{
                err::FinanceError::InvalidPrice,
                "Invalid price format for " + priceStr + ": " + e.what()
            };
        }

        const auto time = json::safeGet<int64_t>(data, "regularMarketTime");
        const auto timeStamp = Timestamp::fromInt64(time);

        return PriceQuote{
            Cash{currency, price},
            timeStamp,
        };
    }

    /**
     * @brief Get the price of the financial instrument.
     *
     * @return const finance::Cash& The price of the financial instrument.
     */
    [[nodiscard]]
    const finance::Cash& PriceQuote::getPrice() const
    {
        return _price;
    }

}   // namespace finance
