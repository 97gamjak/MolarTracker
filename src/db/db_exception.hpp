#ifndef __DB__DB_EXCEPTION_HPP__
#define __DB__DB_EXCEPTION_HPP__

#include <stdexcept>
#include <string>
#include <utility>

namespace db
{
    /**
     * @brief Exception type for SQLite-related errors
     *
     */
    class SqliteError : public std::runtime_error
    {
       public:
        explicit SqliteError(const std::string message)
            : std::runtime_error(message)
        {
        }
    };

}   // namespace db

#endif   // __DB__DB_EXCEPTION_HPP__
