#ifndef __REPO__SRC__REPO__REPO_ERRORS_HPP__
#define __REPO__SRC__REPO__REPO_ERRORS_HPP__

#include <string>

#include "exceptions/base.hpp"
#include "orm/crud/crud_error.hpp"

namespace repo
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
    };

}   // namespace repo

#endif   // __REPO__SRC__REPO__REPO_ERRORS_HPP__