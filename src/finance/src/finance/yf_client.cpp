#include "finance/yf_client.hpp"

#include <nlohmann/json.hpp>
#include <string>

#include "finance/ticker_info.hpp"
#include "http/http_client.hpp"

namespace finance
{

    namespace
    {

        // Yahoo wraps numeric values as { "raw": 123.4, "fmt": "123.40" }

    }   // namespace

    /**
     * @brief Build an HTTP request for the Yahoo Finance API.
     *
     * @param path The API endpoint path.
     * @return http::HttpRequest The constructed HTTP request.
     */
    http::HttpRequest YahooFinanceClient::_buildRequest(const std::string& path)
    {
        return http::HttpRequest{
            .url     = std::string{_baseUrl} + path,
            .headers = {
                {"User-Agent", std::string{_userAgent}},
                {"Accept", "application/json"},
            },
        };
    }

    /**
     * @brief Fetch ticker information from Yahoo Finance API.
     *
     * @param ticker The ticker symbol of the stock.
     * @return std::expected<TickerInfo, http::HttpError>
     */
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
            return TickerInfo::fromJson(json);
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