#include "exception.hpp"

namespace app
{
    ProfileStoreException::ProfileStoreException(std::string message)
        : MolarTrackerException(std::move(message))
    {
    }

    [[nodiscard]] const char* ProfileStoreException::what() const noexcept
    {
        const std::string  prefix = "ProfileStoreException: ";
        static std::string fullMessage;

        fullMessage = prefix + MolarTrackerException::what();
        return fullMessage.c_str();
    }
}   // namespace app