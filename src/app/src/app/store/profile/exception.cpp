#include "exception.hpp"

namespace app
{
    /**
     * @brief Construct a new Profile Store Exception:: Profile Store Exception
     * object
     *
     * @param message
     */
    ProfileStoreException::ProfileStoreException(std::string message)
        : MolarTrackerException(std::move(message))
    {
    }

    /**
     * @brief Get the what message for this exception, prefixed with
     * "ProfileStoreException: "
     *
     * @return const char*
     */
    [[nodiscard]] const char* ProfileStoreException::what() const noexcept
    {
        const std::string  prefix = "ProfileStoreException: ";
        static std::string fullMessage;

        fullMessage = prefix + MolarTrackerException::what();
        return fullMessage.c_str();
    }
}   // namespace app