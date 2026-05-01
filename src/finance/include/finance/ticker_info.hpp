#ifndef __FINANCE__INCLUDE__FINANCE__TICKER_INFO_HPP__
#define __FINANCE__INCLUDE__FINANCE__TICKER_INFO_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <string>

namespace finance
{

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASSET_CLASS_LIST(X) \
    X(Stock)                \
    X(Etf)                  \
    X(Crypto)               \
    X(MutualFund)           \
    X(Future)               \
    X(Unknown)

    MSTD_ENUM(AssetClass, std::uint8_t, ASSET_CLASS_LIST)

    [[nodiscard]]
    AssetClass fromQuote(std::string_view quote);

    [[nodiscard]]
    std::string toString(AssetClass assetClass);

    struct TickerInfo
    {
        std::string symbol;
        std::string shortName;
        std::string longName;
        std::string exchange;
        std::string currency;
        std::string industry;
        std::string sector;
        double      regularMarketPrice{};
        double      previousClose{};
        AssetClass  assetClass{AssetClass::Unknown};
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TICKER_INFO_HPP__