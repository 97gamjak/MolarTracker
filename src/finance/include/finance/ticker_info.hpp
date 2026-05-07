#ifndef __FINANCE__INCLUDE__FINANCE__TICKER_INFO_HPP__
#define __FINANCE__INCLUDE__FINANCE__TICKER_INFO_HPP__

#include <cstdint>
#include <expected>
#include <mstd/enum.hpp>
#include <nlohmann/json.hpp>
#include <string>

#include "config/error.hpp"
#include "config/finance.hpp"

namespace finance
{

    [[nodiscard]]
    AssetClass fromQuote(std::string_view quote);

    [[nodiscard]]
    std::string toString(AssetClass assetClass);

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define FINANCE_ERROR_LIST(X) \
    X(CurrencyUnknown)        \
    X(Unknown)

    MSTD_ENUM(FinanceErrorType, std::uint8_t, FINANCE_ERROR_LIST);

    using FinanceError = Error<FinanceErrorType>;

    /**
     * @brief Structure representing information about a stock ticker.
     *
     */
    struct TickerInfo
    {
        /// The ticker symbol of the stock
        std::string symbol;
        /// The short name of the stock
        std::string shortName;
        /// The long name of the stock
        std::string longName;
        /// The exchange where the stock is listed
        std::string exchange;
        /// The currency of the stock
        Currency currency;
        /// The industry of the stock
        std::string industry;
        /// The sector of the stock
        std::string sector;
        /// The regular market price of the stock
        double regularMarketPrice{};
        /// The previous close price of the stock
        double previousClose{};
        /// The asset class of the stock
        AssetClass assetClass{AssetClass::Unknown};

        [[nodiscard]]
        static std::expected<TickerInfo, FinanceError> fromJson(
            const nlohmann::json& json
        );
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TICKER_INFO_HPP__