#include "orm/crud.hpp"

namespace orm
{
    [[nodiscard]] const std::vector<std::string>& Crud::getExecutedSQL() const
    {
        return _sqlExecutions;
    }

}   // namespace orm
