#include "exceptions/file_exceptions.hpp"

/**
 * @brief Constructs a new Dir Exception object
 * @param message The exception message
 */
DirException::DirException(const std::string& message)
    : MolarTrackerException(message)
{
}