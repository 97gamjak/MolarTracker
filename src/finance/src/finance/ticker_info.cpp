#include "finance/ticker_info.hpp"

#include "json/json.hpp"

namespace finance
{
    /**
     * @brief Convert a string representation of a quote type to an AssetClass.
     *
     * @param quote The string representation of the quote type.
     * @return AssetClass The corresponding AssetClass.
     */
    AssetClass fromQuote(std::string_view quote)
    {
        if (quote == "EQUITY")
            return AssetClass::Stock;
        if (quote == "ETF")
            return AssetClass::Etf;
        if (quote == "Crypto")
            return AssetClass::Crypto;
        if (quote == "MutualFund")
            return AssetClass::MutualFund;
        if (quote == "Future")
            return AssetClass::Future;

        return AssetClass::Unknown;
    }

    namespace
    {
        /**
         * @brief Safely get a double value from a JSON object.
         *
         * @param obj The JSON object to search.
         * @param key The key to look for.
         * @return double The found value or 0.0 if not found.
         */
        double _rawValue(const nlohmann::json& obj, const char* key)
        {
            if (const auto it = obj.find(key);
                it != obj.end() && it->is_object())
            {
                if (const auto raw = it->find("raw"); raw != it->end())
                    return raw->get<double>();
            }
            return {};
        }
    }   // namespace

    /**
     * @brief Create a TickerInfo object from a JSON object.
     *
     * @param json The JSON object containing ticker information.
     * @return std::expected<TickerInfo, FinanceError>
     */
    std::expected<TickerInfo, FinanceError> TickerInfo::fromJson(
        const nlohmann::json& json
    )
    {
        // quoteSummary.result[0].price
        const auto& price =
            json.at("quoteSummary").at("result").at(0).at("price");

        // quoteSummary.result[0].assetProfile — may be absent for
        // ETFs/crypto
        const auto& result     = json.at("quoteSummary").at("result").at(0);
        const bool  hasProfile = result.contains("assetProfile");
        const auto& profile =
            hasProfile ? result.at("assetProfile") : nlohmann::json::object();

        TickerInfo info;
        info.symbol    = json::safeGet<std::string>(price, "symbol");
        info.shortName = json::safeGet<std::string>(price, "shortName");
        info.longName  = json::safeGet<std::string>(price, "longName");
        info.exchange  = json::safeGet<std::string>(price, "exchangeName");

        const auto currencyStr = json::safeGet<std::string>(price, "currency");
        const auto currencyOpt = CurrencyMeta::from_string(currencyStr);

        if (!currencyOpt)
        {
            return std::unexpected(FinanceError(
                FinanceErrorType::CurrencyUnknown,
                "Unknown currency " + currencyStr
            ));
        }

        info.currency = currencyOpt.value();

        info.regularMarketPrice = _rawValue(price, "regularMarketPrice");
        info.previousClose = _rawValue(price, "regularMarketPreviousClose");
        info.assetClass =
            fromQuote(json::safeGet<std::string>(price, "quoteType"));
        info.industry = json::safeGet<std::string>(profile, "industry");
        info.sector   = json::safeGet<std::string>(profile, "sector");

        return info;
    }
}   // namespace finance