#ifndef __ORM__ORM_EXCEPTION_HPP__
#define __ORM__ORM_EXCEPTION_HPP__

#include <stdexcept>
#include <string>

namespace orm
{
    /**
     * @brief Exception type for ORM-related errors
     *
     */
    class ORMError : public std::runtime_error
    {
       public:
        explicit ORMError(const std::string message);
    };

}   // namespace orm

#endif   // __ORM__ORM_EXCEPTION_HPP__
