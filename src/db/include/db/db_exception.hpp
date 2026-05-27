#ifndef __DB__INCLUDE__DB__DB_EXCEPTION_HPP__
#define __DB__INCLUDE__DB__DB_EXCEPTION_HPP__

#include <string>

#include "exceptions/base.hpp"

namespace db
{
    /**
     * @brief Exception type for SQLite-related errors
     *
     */
    class SqliteError : public MolarTrackerException
    {
       public:
        explicit SqliteError(const std::string& message);
    };

}   // namespace db

#endif   // __DB__INCLUDE__DB__DB_EXCEPTION_HPP__
