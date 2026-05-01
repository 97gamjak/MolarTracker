#include "finance/yf_client.hpp"

#include <nlohmann/json.hpp>
#include <string>

#include "finance/ticker_info.hpp"
#include "http/http_client.hpp"

namespace finance
{

    namespace
    {

        // Safe JSON field extraction — returns default if field missing or null
        template <typename T>
        T safeGet(const nlohmann::json& obj, const char* key, T fallback = {})
        {
            if (const auto it = obj.find(key);
                it != obj.end() && !it->is_null())
                return it->get<T>();
            return fallback;
        }

        // Yahoo wraps numeric values as { "raw": 123.4, "fmt": "123.40" }
        double rawValue(const nlohmann::json& obj, const char* key)
        {
            if (const auto it = obj.find(key);
                it != obj.end() && it->is_object())
            {
                if (const auto raw = it->find("raw"); raw != it->end())
                    return raw->get<double>();
            }
            return {};
        }

        TickerInfo parseTickerInfo(const nlohmann::json& json)
        {
            // quoteSummary.result[0].price
            const auto& price =
                json.at("quoteSummary").at("result").at(0).at("price");

            // quoteSummary.result[0].assetProfile — may be absent for
            // ETFs/crypto
            const auto& result     = json.at("quoteSummary").at("result").at(0);
            const bool  hasProfile = result.contains("assetProfile");
            const auto& profile    = hasProfile ? result.at("assetProfile")
                                                : nlohmann::json::object();

            TickerInfo info;
            info.symbol    = safeGet<std::string>(price, "symbol");
            info.shortName = safeGet<std::string>(price, "shortName");
            info.longName  = safeGet<std::string>(price, "longName");
            info.exchange  = safeGet<std::string>(price, "exchangeName");
            info.currency  = safeGet<std::string>(price, "currency");
            info.regularMarketPrice = rawValue(price, "regularMarketPrice");
            info.previousClose = rawValue(price, "regularMarketPreviousClose");
            info.assetClass =
                fromQuote(safeGet<std::string>(price, "quoteType"));
            info.industry = safeGet<std::string>(profile, "industry");
            info.sector   = safeGet<std::string>(profile, "sector");

            return info;
        }

    }   // namespace

    http::HttpRequest YahooFinanceClient::_buildRequest(const std::string& path)
    {
        return http::HttpRequest{
            .url     = std::string{k_baseUrl} + path,
            .headers = {
                {"User-Agent", std::string{k_userAgent}},
                {"Accept", "application/json"},
            },
        };
    }

    std::expected<TickerInfo, http::HttpError> YahooFinanceClient::
        fetchTickerInfo(const std::string& ticker)
    {
        const std::string path = "/v10/finance/quoteSummary/" + ticker +
                                 "?modules=price,assetProfile";

        auto result = http::HttpClient::get(_buildRequest(path));
        if (!result)
            return std::unexpected(result.error());

        try
        {
            const auto json = nlohmann::json::parse(result->body);
            return parseTickerInfo(json);
        }
        catch (const nlohmann::json::exception& ex)
        {
            return std::unexpected(
                http::HttpError{
                    .kind    = http::HttpErrorKind::ParseError,
                    .message = ex.what(),
                }
            );
        }
    }

}   // namespace finance