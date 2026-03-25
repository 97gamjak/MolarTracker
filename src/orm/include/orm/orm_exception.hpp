#ifndef __ORM__ORM_EXCEPTION_HPP__
#define __ORM__ORM_EXCEPTION_HPP__

#include <string>

#include "exceptions/base.hpp"

namespace orm
{
    /**
     * @brief Exception type for ORM-related errors
     *
     */
    class ORMError : public MolarTrackerException
    {
       public:
        explicit ORMError(const std::string& message);
    };

}   // namespace orm

#endif   // __ORM__ORM_EXCEPTION_HPP__
