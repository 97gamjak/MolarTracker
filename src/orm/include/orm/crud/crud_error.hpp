#ifndef __ORM__INCLUDE__ORM__CRUD__CRUD_ERROR_HPP__
#define __ORM__INCLUDE__ORM__CRUD__CRUD_ERROR_HPP__

#include <mstd/enum.hpp>
#include <string>

#include "exceptions/base.hpp"

namespace orm
{

    /**
     * @brief Exception class for CRUD operation errors
     *
     */
    class CrudException : public MolarTrackerException
    {
       public:
        explicit CrudException(const std::string& message);

        [[nodiscard]] const char* what() const noexcept override;
    };

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__CRUD__CRUD_ERROR_HPP__
