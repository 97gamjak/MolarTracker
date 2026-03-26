#ifndef __APP__SRC__APP__REPOS__REPO_ERRORS_HPP__
#define __APP__SRC__APP__REPOS__REPO_ERRORS_HPP__

#include <string>

#include "orm/crud/crud_error.hpp"

namespace app
{
    std::string getInsertError(
        const orm::CrudError& error,
        const std::string&    what
    );

}   // namespace app

#endif   // __APP__SRC__APP__REPOS__REPO_ERRORS_HPP__