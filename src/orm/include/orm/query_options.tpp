#ifndef __ORM__INCLUDE__ORM__QUERY_OPTIONS_TPP__
#define __ORM__INCLUDE__ORM__QUERY_OPTIONS_TPP__

#include "query_options.hpp"

namespace orm
{
    template <typename... Fields>
    std::string QueryOptions<Fields...>::getDBOperations() const
    {
        std::string operations = "ORDER BY";

        return operations;
    }
}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__QUERY_OPTIONS_TPP__
