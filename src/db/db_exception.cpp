#include "db_exception.hpp"

namespace db
{
    /**
     * @brief Construct a new Sqlite Error:: Sqlite Error object
     *
     * @param message The error message
     */
    SqliteError::SqliteError(const std::string& message)
        : std::runtime_error(message)
    {
    }

}   // namespace db