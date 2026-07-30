#include "settings/params/param_error.hpp"

namespace settings
{
    /**
     * @brief Construct a new Param Exception:: Param Exception object
     *
     * @param message The exception message
     */
    ParamException::ParamException(const std::string& message)
        : MolarTrackerException(message)
    {
    }

    /**
     * @brief Get the exception message
     *
     * @return const char* The exception message
     */
    const char* ParamException::what() const noexcept
    {
        static const std::string        prefix = "ParamException: ";
        static thread_local std::string fullMessage;
        fullMessage = prefix + MolarTrackerException::what();
        return fullMessage.c_str();
    }

}   // namespace settings