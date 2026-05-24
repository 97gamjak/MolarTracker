#ifndef __REPO__INCLUDE__REPO__EXCEPTIONS_HPP__
#define __REPO__INCLUDE__REPO__EXCEPTIONS_HPP__

#include "exceptions/base.hpp"

namespace repo
{
    /**
     * @brief Exception thrown when a migration fails
     *
     */
    class MigrationException : public MolarTrackerException
    {
       private:
        /// The database path
        std::string _dbPath;

       public:
        MigrationException(const std::string& message, std::string dbPath);

        [[nodiscard]] const char* what() const noexcept override;
    };
}   // namespace repo

#endif   // __REPO__INCLUDE__REPO__EXCEPTIONS_HPP__
