#include "finance/price_quote.hpp"

#include <expected>
#include <stdexcept>

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

        const auto currencyStr = json::safeGet<std::string>(data, "currency");
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
            json::safeGet<std::string>(data, "regularMarketPreviousClose");

        micro_units price = 0;

        try
        {
            price = microUnitsFromString(priceStr, getMicroUnit(currency));
        }
        catch (const std::overflow_error& e)
        {
            return std::unexpected(FinanceError(
                FinanceErrorType::PriceOverflow,
                "Invalid price " + priceStr + ": " + e.what()
            ));
        }
        catch (const std::invalid_argument& e)
        {
            return std::unexpected(FinanceError(
                FinanceErrorType::InvalidPriceString,
                "Invalid price " + priceStr + ": " + e.what()
            ));
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
