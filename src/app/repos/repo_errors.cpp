#include "repo_errors.hpp"

#include "orm/crud/crud_error.hpp"

namespace app
{

    /**
     * @brief Get the Insert Error message
     *
     * @param error
     * @param what
     * @return std::string
     */
    std::string getInsertError(
        const orm::CrudError& error,
        const std::string&    what
    )
    {
        std::string msg;
        msg += "Failed to insert " + what + ": ";
        msg += error.getMessage() + " (type: ";
        msg += orm::CrudErrorTypeMeta::toString(error.getType()) + ")";

        return msg;
    }

}   // namespace app