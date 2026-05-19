#ifndef __APP__SRC__APP__REPOS__REPO_ERRORS_HPP__
#define __APP__SRC__APP__REPOS__REPO_ERRORS_HPP__

#include <string>

#include "exceptions/base.hpp"
#include "orm/crud/crud_error.hpp"

namespace app
{
    std::string getInsertError(
        const orm::CrudError& error,
        const std::string&    what
    );

    /**
     * @brief Exception class for repository errors.
     */
    class RepositoryException : public MolarTrackerException
    {
       public:
        explicit RepositoryException(const std::string& message);

        [[nodiscard]]
        const char* what() const noexcept override;
    };

}   // namespace app

#endif   // __APP__SRC__APP__REPOS__REPO_ERRORS_HPP__