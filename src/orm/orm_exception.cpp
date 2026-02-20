#include "orm_exception.hpp"

namespace orm
{
    /**
     * @brief Construct a new ORMError:: ORMError object
     *
     * @param message The error message describing the ORM error
     */
    ORMError::ORMError(const std::string message) : std::runtime_error(message)
    {
    }

}   // namespace orm