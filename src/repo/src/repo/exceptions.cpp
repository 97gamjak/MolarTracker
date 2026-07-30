#include "repo/exceptions.hpp"

namespace repo
{
    /**
     * @brief Construct a new Migration Exception:: Migration Exception object
     *
     * @param message
     * @param dbPath
     */
    MigrationException::MigrationException(
        const std::string& message,
        std::string        dbPath
    )
        : MolarTrackerException(message), _dbPath(std::move(dbPath))
    {
    }

    /**
     * @brief Get the error message
     *
     * @return const char* The error message
     */
    const char* MigrationException::what() const noexcept
    {
        const std::string prefix =
            "MigrationException for database (" + _dbPath + "): ";
        static std::string fullMessage;

        fullMessage = prefix + MolarTrackerException::what();
        return fullMessage.c_str();
    }
}   // namespace repo