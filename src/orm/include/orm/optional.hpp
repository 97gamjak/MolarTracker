#ifndef __ORM__INCLUDE__ORM__OPTIONAL_HPP__
#define __ORM__INCLUDE__ORM__OPTIONAL_HPP__

#include <optional>

#include "type_traits.hpp"

namespace orm
{
    /**
     * @brief Class representing an optional Model value
     *
     */
    template <db_model Model>
    struct Optional
    {
        /// the type of the Model
        using value_type = Model;

        /// the actual optional Model object
        std::optional<Model> value;
    };

}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__OPTIONAL_HPP__