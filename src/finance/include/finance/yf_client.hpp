#ifndef __FINANCE__INCLUDE__FINANCE__YF_CLIENT_HPP__
#define __FINANCE__INCLUDE__FINANCE__YF_CLIENT_HPP__

#include <http/http_client.hpp>
#include <string>

#include "finance/ticker_info.hpp"

namespace finance
{

    class YahooFinanceClient
    {
       private:
        static constexpr std::string_view k_baseUrl =
            "https://query2.finance.yahoo.com";
        static constexpr std::string_view k_userAgent =
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64)";

       public:
        [[nodiscard]]
        static std::expected<TickerInfo, http::HttpError> fetchTickerInfo(
            const std::string& ticker
        );

       private:
        [[nodiscard]]
        static http::HttpRequest _buildRequest(const std::string& path);
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__YF_CLIENT_HPP__