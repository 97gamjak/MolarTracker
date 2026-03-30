#include "orm/crud.hpp"

namespace orm
{
    /**
     * @brief Get the SQL statements that have been executed
     *
     * @return const std::vector<std::string>& A reference to the vector of
     * executed SQL statements
     */
    [[nodiscard]] const std::vector<std::string>& Crud::getExecutedSQL() const
    {
        return _sqlExecutions;
    }

}   // namespace orm
