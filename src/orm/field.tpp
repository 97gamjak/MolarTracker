#ifndef __ORM__FIELD_TPP__
#define __ORM__FIELD_TPP__

#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "orm/binder.hpp"
#include "orm/concepts.hpp"
#include "orm/field.hpp"
#include "orm/fixed_string.hpp"
#include "orm/sql_type.hpp"

namespace orm
{
    /**
     * @brief Construct a Field with the specified value
     *
     * @tparam Name
     * @tparam Value
     * @tparam Options
     * @param value
     */
    template <fixed_string Name, typename Value, typename... Options>
    Field<Name, Value, Options...>::Field(Value value)
        : _value(std::move(value))
    {
    }

    /**
     * @brief Get the value of the field
     *
     * @tparam Name
     * @tparam Value
     * @tparam Options
     * @return Value&
     */
    template <fixed_string Name, typename Value, typename... Options>
    Value& Field<Name, Value, Options...>::value()
    {
        return _value;
    }

    /**
     * @brief Get the value of the field (const version)
     *
     * @tparam Name
     * @tparam Value
     * @tparam Options
     * @return Value const&
     */
    template <fixed_string Name, typename Value, typename... Options>
    Value const& Field<Name, Value, Options...>::value() const
    {
        return _value;
    }

    /**
     * @brief Assign a new value to the field
     *
     * @tparam Name
     * @tparam Value
     * @tparam Options
     * @param value
     * @return Field&
     */
    template <fixed_string Name, typename Value, typename... Options>
    Field<Name, Value, Options...>& Field<Name, Value, Options...>::operator=(
        Value value
    )
    {
        _value = std::move(value);
        return *this;
    }

    /**
     * @brief Get the DDL string for the field
     *
     * @tparam Name
     * @tparam Value
     * @tparam Options
     * @return std::string
     */
    template <fixed_string Name, typename Value, typename... Options>
    std::string Field<Name, Value, Options...>::ddl()
    {
        using storage_type = std::
            conditional_t<is_optional_v<Value>, optional_inner_t<Value>, Value>;

        std::string definition;
        definition += std::string{name.view()};
        definition += " ";
        definition += std::string{sql_type<storage_type>::name};

        if constexpr (isPk)
            definition += " PRIMARY KEY";
        if constexpr (isAutoIncrement)
            definition += " AUTOINCREMENT";
        if constexpr (isUnique)
            definition += " UNIQUE";
        if constexpr (!isNullable)
            definition += " NOT NULL";

        return definition;
    }

    /**
     * @brief Bind the field value to the specified parameter index
     *
     * @tparam Name
     * @tparam Value
     * @tparam Options
     * @tparam Statement
     * @param statement
     * @param index
     */
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
                statement.bindNull(index);
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

    /**
     * @brief Read the field value from the specified column
     *
     * @tparam Name
     * @tparam Value
     * @tparam Options
     * @tparam Statement
     * @param statement
     * @param col
     */
    template <fixed_string Name, typename Value, typename... Options>
    template <typename Statement>
    void Field<Name, Value, Options...>::readFrom(
        Statement const& statement,
        int              col
    )
    {
        if constexpr (is_optional_v<Value>)
        {
            using inner_type = optional_inner_t<Value>;

            if (statement.columnIsNull(col))
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