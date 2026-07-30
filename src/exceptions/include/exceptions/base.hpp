#ifndef __EXCEPTIONS__INCLUDE__EXCEPTIONS__BASE_HPP__
#define __EXCEPTIONS__INCLUDE__EXCEPTIONS__BASE_HPP__

#include <stacktrace>
#include <stdexcept>
#include <string>

/**
 * @brief Base class for all application exceptions
 *
 * This class serves as the base for all exceptions thrown within the Molar
 * Tracker application. It inherits from std::exception and provides a
 * constructor that accepts a message string, which can be retrieved using the
 * what() method. This allows for consistent exception handling and messaging
 * throughout the application.
 *
 */
class MolarTrackerException : public std::runtime_error
{
   private:
    /// The stack trace captured at the point where the exception was thrown.
    std::stacktrace _trace;
    /// The full exception message, including the stack trace, returned by
    /// what().
    std::string _what;

   public:
    explicit MolarTrackerException(
        const std::string& msg,
        std::stacktrace    trace = std::stacktrace::current()
    );

    [[nodiscard]]
    const char* what() const noexcept override;

    [[nodiscard]]
    const std::stacktrace& stacktrace() const noexcept;
};

#endif   // __EXCEPTIONS__INCLUDE__EXCEPTIONS__BASE_HPP__