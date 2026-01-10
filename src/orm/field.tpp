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
    Value& Field<Name, Value, Options...>::value()
    {
        return _value;
    }

    template <fixed_string Name, typename Value, typename... Options>
    Value const& Field<Name, Value, Options...>::value() const
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

    template <fixed_string Name, typename Value, typename... Options>
    std::string Field<Name, Value, Options...>::ddl()
    {
        using storage_type = std::
            conditional_t<is_optional_v<Value>, optional_inner_t<Value>, Value>;

        std::string definition;
        definition += std::string{name.view()};
        definition += " ";
        definition += std::string{sql_type<storage_type>::name};

        if constexpr (is_pk)
            definition += " PRIMARY KEY";
        if constexpr (is_auto_increment)
            definition += " AUTOINCREMENT";
        if constexpr (is_unique)
            definition += " UNIQUE";
        if constexpr (!is_nullable)
            definition += " NOT NULL";

        return definition;
    }

    template <fixed_string Name, typename Value, typename... Options>
    template <typename Statement>
    void Field<Name, Value, Options...>::bind(
        Statement& statement,
        int        index
    ) const
    {
        if constexpr (is_optional_v<Value>)
        {
            if (!_value.has_value())
            {
                statement.bind_null(index);
                return;
            }

            using inner_type = optional_inner_t<Value>;
            binder<Statement, inner_type>::bind(statement, index, *_value);
        }
        else
        {
            binder<Statement, Value>::bind(statement, index, _value);
        }
    }

    template <fixed_string Name, typename Value, typename... Options>
    template <typename Statement>
    void Field<Name, Value, Options...>::read_from(
        Statement const& statement,
        int              col
    )
    {
        if constexpr (is_optional_v<Value>)
        {
            using inner_type = optional_inner_t<Value>;

            if (statement.column_is_null(col))
            {
                _value = std::nullopt;
                return;
            }

            _value = binder<Statement, inner_type>::read(statement, col);
        }
        else
        {
            _value = binder<Statement, Value>::read(statement, col);
        }
    }

}   // namespace orm

#endif   // __ORM__FIELD_TPP__