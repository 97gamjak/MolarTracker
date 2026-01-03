#ifndef __ORM__FIELD_TPP__
#define __ORM__FIELD_TPP__

#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "orm/binder.hpp"
#include "orm/concepts.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/fixed_string.hpp"
#include "orm/sql_type.hpp"

namespace orm
{
    template <fixed_string Name, typename Value, typename... Options>
    Field<Name, Value, Options...>::Field(Value value)
        : _value(std::move(value))
    {
    }

    template <fixed_string Name, typename Value, typename... Options>
    Value& Field<Name, Value, Options...>::value() noexcept
    {
        return _value;
    }

    template <fixed_string Name, typename Value, typename... Options>
    Value const& Field<Name, Value, Options...>::value() const noexcept
    {
        return _value;
    }

    template <fixed_string Name, typename Value, typename... Options>
    Field<Name, Value, Options...>& Field<Name, Value, Options...>::operator=(
        Value value
    )
    {
        _value = std::move(value);
        return *this;
    }

}   // namespace orm

#endif   // __ORM__FIELD_TPP__