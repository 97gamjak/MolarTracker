#ifndef __ORM__INCLUDE__ORM__OPTIONAL_HPP__
#define __ORM__INCLUDE__ORM__OPTIONAL_HPP__

#include <optional>

#include "type_traits.hpp"

namespace orm
{
    template <db_model Model>
    struct Optional
    {
        using value_type = Model;
        std::optional<Model> value;
    };

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__OPTIONAL_HPP__