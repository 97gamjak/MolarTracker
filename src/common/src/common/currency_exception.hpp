#ifndef __COMMON__SRC__COMMON__CURRENCY_EXCEPTION_HPP__
#define __COMMON__SRC__COMMON__CURRENCY_EXCEPTION_HPP__

#include <string>

#include "exceptions/base.hpp"

/**
 * @brief Exception thrown when an operation is attempted between
 * incompatible currencies.
 *
 * This exception is used to indicate that a financial operation (e.g.,
 * addition, comparison) was attempted between two Cash objects that have
 * different currencies, which is not allowed.
 */
class CurrencyMismatchException : public MolarTrackerException
{
   public:
    explicit CurrencyMismatchException(const std::string& message);

    [[nodiscard]] const char* what() const noexcept override;
};

#endif   // __COMMON__SRC__COMMON__CURRENCY_EXCEPTION_HPP__