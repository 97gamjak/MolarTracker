#ifndef __ORM__INCLUDE__ORM__QUERY_OPTIONS_HPP__
#define __ORM__INCLUDE__ORM__QUERY_OPTIONS_HPP__

#include <cstddef>
#include <mstd/enum.hpp>

namespace orm
{
    template <typename... Fields>
    struct QueryOptions
    {
        std::optional<std::size_t> limit;

        [[nodiscard]] std::string getDBOperations() const;
    };
}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__QUERY_OPTIONS_HPP__