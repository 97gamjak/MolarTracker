#include "finance/yf_client.hpp"

#include <nlohmann/json.hpp>
#include <string>

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
     * @return std::expected<void, http::HttpError>
     */
    std::expected<void, http::HttpError> YahooSession::authenticate()
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
        else if (cookieResult.error().kind == http::HttpErrorKind::BadStatus)
        {
            headers = &cookieResult.error().responseHeaders;
        }
        else
        {
            return std::unexpected(cookieResult.error());
        }

        const auto it = headers->find("set-cookie");
        if (it == headers->end())
        {
            return std::unexpected(
                http::HttpError{
                    .kind            = http::HttpErrorKind::AuthError,
                    .message         = "No Set-Cookie header from fc.yahoo.com",
                    .responseHeaders = *headers,
                }
            );
        }

        const std::string prefix = "A3=";
        const std::string bValue = extractSessionCookie(it->second, prefix);
        if (bValue.empty())
        {
            return std::unexpected(
                http::HttpError{
                    .kind    = http::HttpErrorKind::AuthError,
                    .message = "B cookie not found in Set-Cookie header",
                    .responseHeaders = *headers,
                }
            );
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
            return std::unexpected(
                http::HttpError{
                    .kind    = http::HttpErrorKind::AuthError,
                    .message = "Empty crumb returned from Yahoo Finance",
                    .responseHeaders = std::move(crumbResult->headers),
                }
            );
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
                    .kind            = http::HttpErrorKind::ParseError,
                    .message         = ex.what(),
                    .responseHeaders = std::move(result->headers),
                }
            );
        }
    }

}   // namespace finance