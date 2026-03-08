#include "log_exceptions.hpp"

namespace logging
{
    /**
     * @brief Construct a new Log Exception:: Log Exception object
     *
     * @param message The error message describing the exception
     */
    LogException::LogException(const std::string& message)
        : MolarTrackerException(message)
    {
    }

    /**
     * @brief Get the error message associated with the exception
     *
     * @return const char* The error message as a C-style string
     */
    const char* LogException::what() const noexcept
    {
        static const std::string prefix = "LogException: ";

        static thread_local std::string fullMessage;

        fullMessage = prefix + MolarTrackerException::what();

        return fullMessage.c_str();
    }

}   // namespace logging