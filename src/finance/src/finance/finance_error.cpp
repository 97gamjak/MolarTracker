#include "finance/finance_error.hpp"

#include <utility>

namespace finance
{
    /**
     * @brief Construct a new Yahoo Finance Error:: Yahoo Finance Error object
     *
     * @param type
     * @param message
     * @param httpError
     */
    YahooFinanceError::YahooFinanceError(
        YahooFinanceError::ErrorType   type,
        std::string                    message,
        std::optional<http::HttpError> httpError
    )
        : YahooFinanceErrorBase(type, std::move(message)),
          _httpError(std::move(httpError))
    {
    }

    /**
     * @brief Converts a FinanceError to a YahooFinanceError.
     *
     * @param error The FinanceError to convert.
     * @return YahooFinanceError The converted YahooFinanceError.
     */
    YahooFinanceError YahooFinanceError::fromError(const FinanceError& error)
    {
        // TODO(97gamjak): think here of a better logic rather than adding all
        // types twice
        switch (error.getType())
        {
            case FinanceErrorType::CurrencyUnknown:
                return YahooFinanceError{
                    YahooFinanceErrorType::CurrencyUnknown,
                    error.getMessage()
                };
            case FinanceErrorType::Unknown:
                return YahooFinanceError{
                    YahooFinanceErrorType::Unknown,
                    error.getMessage()
                };
            case finance::FinanceErrorType::PriceOverflow:
                return YahooFinanceError{
                    YahooFinanceErrorType::PriceOverflow,
                    error.getMessage()
                };
            case finance::FinanceErrorType::InvalidPriceString:
                return YahooFinanceError{
                    YahooFinanceErrorType::InvalidPriceString,
                    error.getMessage()
                };
        }

        std::unreachable();
    }
}   // namespace finance