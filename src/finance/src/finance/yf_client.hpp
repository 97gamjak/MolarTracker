#ifndef __FINANCE__SRC__FINANCE__YF_CLIENT_HPP__
#define __FINANCE__SRC__FINANCE__YF_CLIENT_HPP__

#include <string>

#include "finance/price_quote.hpp"
#include "finance/ticker_info.hpp"
#include "utils/result/error.hpp"

namespace http
{
    class HttpRequest;    // Forward declaration
    class HttpResponse;   // Forward declaration
}   // namespace http

namespace finance
{
    class YahooFinanceError;   // Forward declaration

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
        [[nodiscard]] MTResult<void> authenticate();

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
        static MTResult<TickerInfo> fetchTickerInfo(const std::string& ticker);

        [[nodiscard]]
        static MTResult<PriceQuote> fetchPrice(const std::string& ticker);

       private:
        [[nodiscard]]
        static http::HttpRequest _buildRequest(const std::string& path);

        [[nodiscard]]
        static MTResult<http::HttpResponse> _getRequest(
            const std::string& path
        );
    };

}   // namespace finance

#endif   // __FINANCE__SRC__FINANCE__YF_CLIENT_HPP__