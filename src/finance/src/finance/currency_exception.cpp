#include "currency_exception.hpp"

namespace finance
{

    /**
     * @brief Construct a new CurrencyMismatchException::
     * CurrencyMismatchException
     *
     * @param message the error message describing the currency mismatch
     */
    CurrencyMismatchException::CurrencyMismatchException(
        const std::string& message
    )
        : MolarTrackerException(message)
    {
    }

    /**
     * @brief Get the error message for the exception
     *
     * @return const char* the error message describing the currency mismatch
     */
    const char* CurrencyMismatchException::what() const noexcept
    {
        static const std::string        prefix = "Currency Mismatch: ";
        static thread_local std::string fullMessage;

        fullMessage = prefix + MolarTrackerException::what();

        return fullMessage.c_str();
    }

}   // namespace finance