#ifndef __ERROR__INCLUDE__ERROR__FINANCE_ERROR_HPP__
#define __ERROR__INCLUDE__ERROR__FINANCE_ERROR_HPP__

#include <mstd/enum.hpp>

#include "error/base_error.hpp"
#include "error/http_error.hpp"

#define YFINANCE_ERROR_TYPE_LIST(X) \
    X(HttpError)                    \
    X(InvalidTicker)                \
    X(InvalidPriceQuote)            \
    GENERIC_ERRORS(X)

#define FINANCE_ERROR_TYPE_LIST(X) \
    X(StockNotFound)               \
    X(AccountNotFound)             \
    X(InvalidAccountType)          \
    X(InvalidTransaction)          \
    X(InvalidAccount)              \
    X(InvalidStock)                \
    X(InvalidOption)               \
    X(InvalidPosition)             \
    X(CurrencyUnknown)             \
    X(InvalidPriceString)          \
    X(PriceOverflow)               \
    X(UnknownOption)               \
    X(PnlError)                    \
    GENERIC_ERRORS(X)

#define PNL_ERROR_TYPE_LIST(X)  \
    X(InconsistentContractSize) \
    X(UnknownOption)            \
    GENERIC_ERRORS(X)

#define WATCHLIST_ERROR_TYPE_LIST(X) \
    X(WatchlistNotFound)             \
    X(WatchlistAlreadyExists)        \
    X(WatchlistSymbolAlreadyExists)  \
    X(WatchlistSymbolNotFound)       \
    GENERIC_ERRORS(X)

MSTD_ENUM(FinanceErrorType, std::uint8_t, FINANCE_ERROR_TYPE_LIST);
MSTD_ENUM(YFinanceErrorType, std::uint8_t, YFINANCE_ERROR_TYPE_LIST);
MSTD_ENUM(PnLErrorType, std::uint8_t, PNL_ERROR_TYPE_LIST);
MSTD_ENUM(WatchlistErrorType, std::uint8_t, WATCHLIST_ERROR_TYPE_LIST);

using FinanceError   = Error<FinanceErrorType>;
using YFinanceError  = Error<YFinanceErrorType>;
using PnLError       = Error<PnLErrorType>;
using WatchlistError = Error<WatchlistErrorType>;

template <typename T>
using FinanceResult = Result<T, FinanceError>;

template <typename T>
using YFinanceResult = Result<T, YFinanceError>;

template <typename T>
using PnLResult = Result<T, PnLError>;

template <typename T>
using WatchlistResult = Result<T, WatchlistError>;

/**
 * @brief Converts an error of type HttpError to an error of type YFinanceError,
 * preserving the error message and sub-errors.
 */
template <>
struct FromError<HttpError, YFinanceError>
{
    /**
     * @brief Converts an error of type HttpError to an error of type
     * YFinanceError, preserving the error message and sub-errors.
     *
     * @param error The original HttpError to convert.
     * @param newMessage An optional new error message to use for the converted
     * error. If not provided, the original error message will be used.
     * @return YFinanceError A new YFinanceError object with the same message
     * and sub-errors as the original HttpError.
     */
    // NOLINTNEXTLINE(misc-no-recursion)
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
