#ifndef __ORM__ORM_EXCEPTION_HPP__
#define __ORM__ORM_EXCEPTION_HPP__

#include <stdexcept>
#include <string>
#include <utility>

namespace orm
{
    /**
     * @brief Exception type for ORM-related errors
     *
     */
    class ORMError : public std::runtime_error
    {
       public:
        explicit ORMError(const std::string message)
            : std::runtime_error(message)
        {
        }
    };

}   // namespace orm

#endif   // __ORM__ORM_EXCEPTION_HPP__
