#ifndef __FINANCE__INCLUDE__FINANCE__YF_CLIENT_HPP__
#define __FINANCE__INCLUDE__FINANCE__YF_CLIENT_HPP__

#include <string>

#include "finance/ticker_info.hpp"
#include "http/http_error.hpp"

namespace http
{
    class HttpRequest;   // Forward declaration
}   // namespace http

namespace finance
{
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define YAHOO_FINANCE_ERROR(X) \
    X(HttpError)               \
    X(CurrencyUnknown)         \
    X(Unknown)

    MSTD_ENUM(YahooFinanceErrorType, std::uint8_t, YAHOO_FINANCE_ERROR);

    using YahooFinanceErrorBase = Error<YahooFinanceErrorType>;

    /**
     * @brief Represents an error returned by the Yahoo Finance API.
     *
     */
    class YahooFinanceError : public YahooFinanceErrorBase
    {
       private:
        /// optional http error from which this error results from
        std::optional<http::HttpError> _httpError;

       public:
        YahooFinanceError(
            ErrorType                      type,
            std::string                    message,
            std::optional<http::HttpError> httpError = std::nullopt
        );

        static YahooFinanceError fromError(const FinanceError& error);
    };

    /**
     * @brief Yahoo Finance API credentials
     *
     */
    struct YahooCredentials
    {
        /// Yahoo Finance API cookie
        std::string cookie;
        /// Yahoo Finance API crumb
        std::string crumb;
    };

    /**
     * @brief Yahoo Finance API session
     *
     */
    class YahooSession
    {
       private:
        /// Yahoo Finance API credentials
        YahooCredentials _credentials;
        /// Whether the session is authenticated
        bool _authenticated = false;

       public:
        // Fetches cookie + crumb. Call once before any ticker requests.
        [[nodiscard]] std::expected<void, http::HttpError> authenticate();

        [[nodiscard]] bool isAuthenticated() const;

        [[nodiscard]] const YahooCredentials& credentials() const;
    };

    /**
     * @brief Client for interacting with the Yahoo Finance API.
     *
     */
    class YahooFinanceClient
    {
       private:
        /// Base URL for the Yahoo Finance API
        static constexpr std::string_view _baseUrl =
            "https://query2.finance.yahoo.com";
        /// User-Agent header to use for requests
        static constexpr std::string_view _userAgent =
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64)";

       public:
        [[nodiscard]]
        static std::expected<TickerInfo, YahooFinanceError> fetchTickerInfo(
            const std::string& ticker
        );

       private:
        [[nodiscard]]
        static http::HttpRequest _buildRequest(const std::string& path);
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__YF_CLIENT_HPP__