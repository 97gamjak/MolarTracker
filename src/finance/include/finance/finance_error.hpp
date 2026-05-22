#ifndef __FINANCE__INCLUDE__FINANCE__FINANCE_ERROR_HPP__
#define __FINANCE__INCLUDE__FINANCE__FINANCE_ERROR_HPP__

#include <cstdint>
#include <mstd/enum.hpp>

#include "config/error.hpp"
#include "http/http_error.hpp"

namespace finance
{
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define FINANCE_ERROR_LIST(X) \
    X(CurrencyUnknown)        \
    X(PriceOverflow)          \
    X(InvalidPriceString)     \
    X(Unknown)

    MSTD_ENUM(FinanceErrorType, std::uint8_t, FINANCE_ERROR_LIST);

    using FinanceError = Error<FinanceErrorType>;

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
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__FINANCE_ERROR_HPP__