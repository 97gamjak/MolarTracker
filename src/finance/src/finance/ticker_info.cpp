#include "finance/ticker_info.hpp"

#include <nlohmann/json.hpp>

namespace finance
{
    AssetClass fromQuote(std::string_view quote)
    {
        if (quote == "Stock")
            return AssetClass::Stock;
        if (quote == "Etf")
            return AssetClass::Etf;
        if (quote == "Crypto")
            return AssetClass::Crypto;
        if (quote == "MutualFund")
            return AssetClass::MutualFund;
        if (quote == "Future")
            return AssetClass::Future;

        return AssetClass::Unknown;
    }

    std::string toString(AssetClass assetClass)
    {
        if (assetClass == AssetClass::MutualFund)
            return "MutualFund";

        return AssetClassMeta::toString(assetClass);
    }

    namespace
    {
        /**
         * @brief Safely get a string value from a JSON object.
         *
         * @tparam T
         * @param obj
         * @param key
         * @param fallback
         * @return T
         */
        template <typename T>
        T _safeGet(const nlohmann::json& obj, const char* key, T fallback = {})
        {
            if (const auto it = obj.find(key);
                it != obj.end() && !it->is_null())
                return it->get<T>();
            return fallback;
        }

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
     * @return TickerInfo The constructed TickerInfo object.
     */
    TickerInfo TickerInfo::fromJson(const nlohmann::json& json)
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
        info.symbol             = _safeGet<std::string>(price, "symbol");
        info.shortName          = _safeGet<std::string>(price, "shortName");
        info.longName           = _safeGet<std::string>(price, "longName");
        info.exchange           = _safeGet<std::string>(price, "exchangeName");
        info.currency           = _safeGet<Currency>(price, "currency");
        info.regularMarketPrice = _rawValue(price, "regularMarketPrice");
        info.previousClose = _rawValue(price, "regularMarketPreviousClose");
        info.assetClass = fromQuote(_safeGet<std::string>(price, "quoteType"));
        info.industry   = _safeGet<std::string>(profile, "industry");
        info.sector     = _safeGet<std::string>(profile, "sector");

        return info;
    }
}   // namespace finance