#ifndef __CONFIG__INCLUDE__CONFIG__DETAILS__ID_TYPES_TPP__
#define __CONFIG__INCLUDE__CONFIG__DETAILS__ID_TYPES_TPP__

#include "config/id_types.hpp"
#include "config/type_traits.hpp"

template <typename T>
auto getId(const T& value)
{
    if constexpr (is_variant_v<T>)
    {
        return std::visit(
            [](const auto& _value) { return _value.getId(); },
            value
        );
    }
    else
    {
        return value.getId();
    }
}

#endif   // __CONFIG__INCLUDE__CONFIG__DETAILS__ID_TYPES_TPP__