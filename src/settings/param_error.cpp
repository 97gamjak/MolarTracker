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
}   // namespace settings