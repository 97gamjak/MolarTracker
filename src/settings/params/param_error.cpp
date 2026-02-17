#include "param_error.hpp"

namespace settings
{
    /**
     * @brief Construct a new Param Error:: Param Error object
     *
     * @param message
     */
    ParamError::ParamError(std::string message) : _message(std::move(message))
    {
    }

    /**
     * @brief Get the error message
     *
     * @return const std::string&
     */
    const std::string& ParamError::getMessage() { return _message; }

    /**
     * @brief Construct a new Param Exception:: Param Exception object
     *
     * @param message The exception message
     */
    ParamException::ParamException(std::string message)
        : MolarTrackerException(std::move(message))
    {
    }

    /**
     * @brief Get the exception message
     *
     * @return const char* The exception message
     */
    const char* ParamException::what() const noexcept
    {
        static const std::string prefix = "ParamException: ";
        static thread_local std::string fullMessage;
        fullMessage = prefix + MolarTrackerException::what();
        return fullMessage.c_str();
    }

}   // namespace settings