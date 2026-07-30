#include "finance/yf_client.hpp"

#include <nlohmann/json.hpp>
#include <string>

#include "error/finance_error.hpp"
#include "error/http_error.hpp"
#include "finance/price_quote.hpp"
#include "finance/ticker_info.hpp"
#include "http/http_client.hpp"

namespace finance
{

    namespace
    {

        /**
         * @brief Extract a specific cookie value from a Set-Cookie header.
         *
         * @param cookieHeader The Set-Cookie header string.
         * @param prefix The cookie name prefix (e.g., "B=").
         * @return std::string The extracted cookie value, or an empty string if
         * not found.
         */
        std::string extractSessionCookie(
            const std::string& cookieHeader,
            const std::string& prefix
        )
        {
            // The cookie name we need is literally "A3="
            const auto start = cookieHeader.find(prefix);
            if (start == std::string::npos)
                return {};
            const auto valueStart = start + prefix.size();
            const auto end        = cookieHeader.find(';', valueStart);
            return cookieHeader.substr(
                valueStart,
                end == std::string::npos ? std::string::npos : end - valueStart
            );
        }

    }   // namespace

    /**
     * @brief Authenticate the Yahoo Finance API session.
     *
     * @return HttpResult<void> Returns an empty result on success, or an error
     * on failure.
     */
    HttpResult<void> YahooSession::authenticate()
    {
        // Step 1: hit fc.yahoo.com to get the B cookie
        auto cookieResult = http::HttpClient::get(
            http::HttpRequest{
                .url = "https://fc.yahoo.com",
                .headers =
                    {
                        {"User-Agent",
                         "Mozilla/5.0 (Windows NT 10.0; Win64; x64)"},
                    },
            }
        );

        const std::map<std::string, std::string>* headers = nullptr;
        if (cookieResult.has_value())
        {
            headers = &cookieResult->headers;
        }
        else if (cookieResult.error().getType() == HttpErrorType::BadStatus)
        {
            headers = &cookieResult.error().getResponseHeaders();
        }
        else
        {
            return std::unexpected(cookieResult.error());
        }

        const auto it = headers->find("set-cookie");
        if (it == headers->end())
        {
            return HttpError{
                HttpErrorType::AuthError,
                "No Set-Cookie header from fc.yahoo.com",
                *headers,
            };
        }

        const std::string prefix = "A3=";
        const std::string bValue = extractSessionCookie(it->second, prefix);
        if (bValue.empty())
        {
            return HttpError{
                HttpErrorType::AuthError,
                "B cookie not found in Set-Cookie header",
                *headers,
            };
        }

        _credentials.cookie = prefix + bValue;

        // Step 2: fetch the crumb using the cookie
        auto crumbResult = http::HttpClient::get(
            http::HttpRequest{
                .url = "https://query2.finance.yahoo.com/v1/test/getcrumb",
                .headers =
                    {
                        {"User-Agent",
                         "Mozilla/5.0 (Windows NT 10.0; Win64; x64)"},
                        {"Cookie", _credentials.cookie},
                    },
            }
        );

        if (!crumbResult)
            return std::unexpected(crumbResult.error());

        _credentials.crumb = crumbResult->body;

        if (_credentials.crumb.empty())
        {
            return HttpError{
                HttpErrorType::AuthError,
                "Empty crumb returned from Yahoo Finance",
                std::move(crumbResult->headers),
            };
        }

        _authenticated = true;
        return {};
    }

    /**
     * @brief Check if the Yahoo Finance API session is authenticated.
     *
     * @return true if authenticated, false otherwise
     */
    [[nodiscard]] bool YahooSession::isAuthenticated() const
    {
        return _authenticated;
    }

    /**
     * @brief Get the Yahoo Finance API credentials.
     *
     * @return const YahooCredentials&
     */
    [[nodiscard]] const YahooCredentials& YahooSession::credentials() const
    {
        return _credentials;
    }

    /**
     * @brief Build an HTTP request for the Yahoo Finance API.
     *
     * @param path The API endpoint path.
     * @return http::HttpRequest The constructed HTTP request.
     */
    http::HttpRequest YahooFinanceClient::_buildRequest(const std::string& path)
    {
        // TODO(97gamjak): keep session alive
        YahooSession session;
        auto         authResult = session.authenticate();

        if (!authResult)
            return http::HttpRequest{};

        const auto& credentials = session.credentials();
        const auto  encodedCrumb =
            http::HttpClient::urlEncode(credentials.crumb);

        return http::HttpRequest{
            .url     = std::string{_baseUrl} + path + "&crumb=" + encodedCrumb,
            .headers = {
                {"User-Agent", std::string{_userAgent}},
                {"Accept", "application/json"},
                {"Cookie", credentials.cookie},
            },
        };
    }

    /**
     * @brief Send a GET request to the Yahoo Finance API.
     *
     * @param path The API endpoint path.
     * @return YFinanceResult<http::HttpResponse>
     */
    YFinanceResult<http::HttpResponse> YahooFinanceClient::_getRequest(
        const std::string& path
    )
    {
        auto result = http::HttpClient::get(_buildRequest(path));
        if (result)
            return result.value();

        return FromError<HttpError, YFinanceError>::apply(result.error());
    }

    /**
     * @brief Fetch ticker information from Yahoo Finance API.
     *
     * @param ticker The ticker symbol of the stock.
     * @return YFinanceResult<TickerInfo>
     */
    YFinanceResult<TickerInfo> YahooFinanceClient::fetchTickerInfo(
        const std::string& ticker
    )
    {
        const std::string path = "/v10/finance/quoteSummary/" + ticker +
                                 "?modules=price,assetProfile";

        const auto result = _getRequest(path);
        if (!result)
            return std::unexpected(result.error());

        try
        {
            const auto json         = nlohmann::json::parse(result->body);
            const auto tickerResult = TickerInfo::fromJson(json);

            if (!tickerResult)
            {
                return FromError<FinanceError, YFinanceError>::apply(
                    tickerResult.error(),
                    YFinanceErrorType::InvalidTicker
                );
            }

            return tickerResult.value();
        }
        catch (const nlohmann::json::exception& ex)
        {
            return FromError<HttpError, YFinanceError>::apply(
                HttpError{
                    HttpErrorType::ParseError,
                    ex.what(),
                    result->headers,
                }
            );
        }

        std::unreachable();
    }

    /**
     * @brief Fetches the latest price quote for a ticker.
     *
     * @param ticker The ticker symbol to look up.
     * @return YFinanceResult<PriceQuote>
     */
    YFinanceResult<PriceQuote> YahooFinanceClient::fetchPrice(
        const std::string& ticker
    )
    {
        const std::string path =
            "/v10/finance/quoteSummary/" + ticker + "?modules=price";

        auto result = _getRequest(path);
        if (!result)
            return result.error();

        try
        {
            const auto json        = nlohmann::json::parse(result->body);
            const auto quoteResult = PriceQuote::fromJson(json);

            if (!quoteResult)
            {
                return FromError<FinanceError, YFinanceError>::apply(
                    quoteResult.error(),
                    YFinanceErrorType::InvalidPriceQuote
                );
            }

            return quoteResult.value();
        }
        catch (const nlohmann::json::exception& ex)
        {
            return FromError<HttpError, YFinanceError>::apply(
                HttpError{
                    HttpErrorType::ParseError,
                    ex.what(),
                    std::move(result->headers),
                }
            );
        }

        std::unreachable();
    }

}   // namespace finance