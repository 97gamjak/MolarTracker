#include "repo_errors.hpp"

namespace repo
{

    /**
     * @brief Construct a new Repository Exception:: Repository Exception object
     *
     * @param message
     */
    RepositoryException::RepositoryException(const std::string& message)
        : MolarTrackerException(message)
    {
    }

}   // namespace repo
