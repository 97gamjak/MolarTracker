#ifndef __DB__DB_EXCEPTION_HPP__
#define __DB__DB_EXCEPTION_HPP__

#include <stdexcept>
#include <string>

namespace db
{
    // TODO: use here MolarTracker's own exception hierarchy, e.g. by deriving
    // from a base MolarTrackerException class
    /**
     * @brief Exception type for SQLite-related errors
     *
     */
    class SqliteError : public std::runtime_error
    {
       public:
        explicit SqliteError(const std::string& message);
    };

}   // namespace db

#endif   // __DB__DB_EXCEPTION_HPP__
