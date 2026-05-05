#include "exceptions/not_yet_implemented.hpp"

/**
 * @brief Construct a new Not Yet Implemented Exception object
 *
 * @param message The message to include in the exception
 */
NotYetImplementedException::NotYetImplementedException(
    const std::string& message
)
    : MolarTrackerException("NotYetImplemented: " + message)
{
}
