#include "crud_error.hpp"

namespace orm
{
    /**
     * @brief Get the type of the CRUD error
     *
     * @return CrudErrorType
     */
    [[nodiscard]] CrudErrorType CrudError::getType() const { return _type; }

    /**
     * @brief Get the error message
     *
     * @return const std::string&
     */
    [[nodiscard]] const std::string& CrudError::getMessage() const
    {
        return _message;
    }

    /**
     * @brief Construct a new CrudException with the specified message
     *
     * @param message
     */
    CrudException::CrudException(const std::string& message)
        : MolarTrackerException(message)
    {
    }

    /**
     * @brief Get the error message with a "CrudException: " prefix
     *
     * @return const char*
     */
    const char* CrudException::what() const noexcept
    {
        static const std::string prefix = "CrudException: ";

        static thread_local std::string fullMessage;
        fullMessage = prefix + MolarTrackerException::what();

        return fullMessage.c_str();
    }

}   // namespace orm