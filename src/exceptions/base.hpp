#ifndef __EXCEPTIONS__BASE_HPP__
#define __EXCEPTIONS__BASE_HPP__

#include <exception>
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
class MolarTrackerException : public std::exception
{
   private:
    /// The error message associated with the exception
    std::string _message;

   public:
    explicit MolarTrackerException(const std::string& message);

    const char* what() const noexcept override;
};

#endif   // __EXCEPTIONS__BASE_HPP__