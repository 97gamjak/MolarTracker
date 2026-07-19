#include "exceptions/out_of_range.hpp"

/**
 * @brief Construct a new Out Of Range object
 *
 * @param message The message to include in the exception
 */
OutOfRange::OutOfRange(const std::string& message)
    : MolarTrackerException("OutOfRange: " + message)
{
}