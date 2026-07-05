#ifndef __ERROR__INCLUDE__ERROR__FINANCE_ERROR_HPP__
#define __ERROR__INCLUDE__ERROR__FINANCE_ERROR_HPP__

#include "error/base_error.hpp"
#include "error/http_error.hpp"

#define YFINANCE_ERROR_TYPE_LIST(X) \
    X(HttpError)                    \
    X(InvalidTicker)                \
    X(InvalidPriceQuote)

#define FINANCE_ERROR_TYPE_LIST(X) \
    X(StockNotFound)               \
    X(AccountNotFound)             \
    X(InvalidTransaction)          \
    X(InvalidAccount)              \
    X(InvalidStock)                \
    X(CurrencyUnknown)             \
    X(InvalidPriceString)          \
    X(PriceOverflow)

MSTD_ENUM(FinanceErrorType, std::uint8_t, FINANCE_ERROR_TYPE_LIST);
MSTD_ENUM(YFinanceErrorType, std::uint8_t, YFINANCE_ERROR_TYPE_LIST);

using FinanceError  = Error<FinanceErrorType>;
using YFinanceError = Error<YFinanceErrorType>;

template <typename T>
using FinanceResult = Result<T, FinanceError>;

template <typename T>
using YFinanceResult = Result<T, YFinanceError>;

template <>
struct FromError<HttpError, YFinanceError>
{
    static YFinanceError apply(
        const HttpError&                  error,
        const std::optional<std::string>& newMessage = std::nullopt
    )
    {
        std::vector<YFinanceError> subErrors;
        for (const auto& subError : error.getSubErrors())
        {
            subErrors.push_back(
                FromError<HttpError, YFinanceError>::apply(
                    dynamic_cast<const HttpError&>(subError)
                )
            );
        }

        return YFinanceError(
            YFinanceErrorType::HttpError,
            newMessage.value_or(ErrorWrapper{error}.getMessage()),
            subErrors
        );
    }
};

#endif   // __ERROR__INCLUDE__ERROR__FINANCE_ERROR_HPP__