#include "exceptions/base.hpp"

/**
 * @brief Constructs a new Molar Tracker Exception object
 *
 * @param message The exception message
 */
MolarTrackerException::MolarTrackerException(const std::string& message)
    : _message{message}
{
}

/**
 * @brief Returns the exception message
 *
 * @return const char* The exception message
 */
const char* MolarTrackerException::what() const noexcept
{
    return _message.c_str();
}