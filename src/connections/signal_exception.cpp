#include "signal_exception.hpp"

/**
 * @brief Construct a new Signal Exception:: Signal Exception object
 *
 * @param message The error message describing the exception
 */
SignalException::SignalException(const std::string& message)
    : MolarTrackerException(message)
{
}

/**
 * @brief Get the error message describing the exception
 *
 * @return const char*
 */
const char* SignalException::what() const noexcept
{
    static const std::string prefix = "SignalException: ";
    static std::string       fullMessage;
    fullMessage = prefix + MolarTrackerException::what();
    return fullMessage.c_str();
}