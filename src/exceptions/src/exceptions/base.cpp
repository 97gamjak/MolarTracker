#include "exceptions/base.hpp"

#include <utility>

/**
 * @brief Constructs a new Molar Tracker Exception object
 *
 * @param message The exception message
 * @param trace The stack trace at the point the exception was thrown,
 * defaulting to the current stack trace
 */
MolarTrackerException::MolarTrackerException(
    const std::string& msg,
    std::stacktrace    trace
)
    : std::runtime_error(msg),
      m_trace(std::move(trace)),
      m_what(
          std::runtime_error::what() + std::string("\nStack trace:\n") +
          std::to_string(m_trace)
      )
{
}

/**
 * @brief Returns the exception message
 *
 * @return const char* The exception message
 */
const char* MolarTrackerException::what() const noexcept
{
    return m_what.c_str();
}

/**
 * @brief Returns the stack trace associated with the exception
 *
 * @return const std::stacktrace& The stack trace
 */
const std::stacktrace& MolarTrackerException::stacktrace() const noexcept
{
    return m_trace;
}