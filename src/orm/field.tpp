#ifndef __ORM__FIELD_TPP__
#define __ORM__FIELD_TPP__

#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "binder.hpp"
#include "concepts.hpp"
#include "constraints.hpp"
#include "field.hpp"
#include "fixed_string.hpp"
#include "sqlite_type.hpp"

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

    template <fixed_string Name, typename Value, typename... Options>
    static std::string Field<Name, Value, Options...>::ddl()
    {
        constexpr static bool is_optional = is_optional_v<Value>;
        using opt_type                    = optional_inner_t<Value>;

        using storage_type = std::conditional_t<is_optional, opt_type, Value>;

        std::string definition;
        definition += std::string{name.view()};
        definition += " ";
        definition += std::string{sqlite_type<storage_type>::name};

        if constexpr (is_primary_key)
            definition += " PRIMARY KEY";
        if constexpr (is_auto_increment)
            definition += " AUTOINCREMENT";
        if constexpr (is_unique)
            definition += " UNIQUE";
        if constexpr (!is_nullable)
            definition += " NOT NULL";

        return definition;
    }

    template <typename StatementLike>
    template <fixed_string Name, typename Value, typename... Options>
    void Field<Name, Value, Options...>::bind(
        StatementLike& statement,
        int            index
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
            binder<StatementLike, inner_type>::bind(statement, index, *_value);
        }
        else
            binder<StatementLike, Value>::bind(statement, index, _value);
    }

    template <typename StatementLike>
    template <fixed_string Name, typename Value, typename... Options>
    void Field<Name, Value, Options...>::read_from(
        StatementLike const& statement,
        int                  col
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

            _value = binder<StatementLike, inner_type>::read(statement, col);
        }
        else
            _value = binder<StatementLike, Value>::read(statement, col);
    }
}   // namespace orm

#endif   // __ORM__FIELD_TPP__