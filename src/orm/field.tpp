#ifndef __ORM__FIELD_TPP__
#define __ORM__FIELD_TPP__

#include <mstd/error.hpp>
#include <mstd/type_traits.hpp>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "config/strong_id.hpp"
#include "index.hpp"
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
     * @brief Get the string representation of the field's value
     *
     * @tparam Name
     * @tparam Value
     * @tparam Options
     * @return std::string
     */
    template <fixed_string Name, typename Value, typename... Options>
    std::string Field<Name, Value, Options...>::valueAsString() const
    {
        return _valueAsString(_value);
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

        if constexpr (isFk)
            definition += "FOREIGN KEY (";

        definition += name.toString();

        if constexpr (isFk)
            definition += ")";

        definition += " ";

        if constexpr (isFk)
            definition += "REFERENCES";
        else
            definition += std::string{sql_type<storage_type>::name};

        if constexpr (isFk)
        {
            using foreignKeyInfo   = find_foreign_key_t<Value, Options...>;
            using field            = typename foreignKeyInfo::Field;
            using table            = typename foreignKeyInfo::Table;
            using DeletionBehavior = typename foreignKeyInfo::DeletionBehavior;

            definition += " " + table::tableName;
            definition += "(" + field::getColumnName() + ")";

            if constexpr (std::same_as<DeletionBehavior, CascadeDelete>)
                definition += " ON DELETE CASCADE";
            else if constexpr (std::same_as<DeletionBehavior, RestrictDelete>)
                definition += " ON DELETE RESTRICT";
            else
                MSTD_COMPILE_FAIL(
                    "Unsupported deletion behavior for foreign key"
                );
        }
        else
        {
            if constexpr (isPk)
                definition += " PRIMARY KEY";
            if constexpr (isAutoIncrement)
                definition += " AUTOINCREMENT";
            if constexpr (isUnique)
                definition += " UNIQUE";
            if constexpr (!isNullable)
                definition += " NOT NULL";
        }

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
        BindIndex  index
    ) const
    {
        if constexpr (is_optional_v<Value>)
        {
            if (!_value.has_value())
            {
                statement.bindNull(index.value());
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
        ColumnIndex      col
    )
    {
        if constexpr (is_optional_v<Value>)
        {
            using inner_type = optional_inner_t<Value>;

            if (statement.columnIsNull(col.value()))
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

    /**
     * @brief Get the column name for the field
     *
     * @tparam Name
     * @tparam Value
     * @tparam Options
     * @return std::string_view
     */
    template <fixed_string Name, typename Value, typename... Options>
    constexpr std::string Field<Name, Value, Options...>::getColumnName()
    {
        return name.toString();
    }

    /**
     * @brief Get the combined constraints for the field
     *
     * @tparam Name
     * @tparam Value
     * @tparam Options
     * @return ORMConstraint
     */
    template <fixed_string Name, typename Value, typename... Options>
    constexpr ORMConstraint Field<Name, Value, Options...>::getConstraints()
    {
        return (ORMConstraint{} | ... | Options::value);
    }

    /**
     * @brief Get the string representation of a field value, handling special
     * cases like optional values, strong IDs, and enums
     *
     * @tparam Name
     * @tparam Value
     * @tparam Options
     * @param value
     * @return std::string
     */
    template <fixed_string Name, typename Value, typename... Options>
    std::string Field<Name, Value, Options...>::_valueAsString(Value value)
    {
        if constexpr (is_optional_v<Value>)
        {
            if (!value.has_value())
                return "null";

            using InnerType = optional_inner_t<Value>;
            return Field<Name, InnerType, Options...>::_valueAsString(
                value.value()
            );
        }
        else if constexpr (isStrongId_v<Value>)
        {
            return value.toString();
        }
        else if constexpr (mstd::has_enum_meta<Value>)
        {
            using enumMeta = mstd::enum_meta_t<Value>;
            return enumMeta::toString(value);
        }
        else if constexpr (std::is_same_v<Value, std::string>)
        {
            return "\"" + value + "\"";
        }
        else
        {
            return std::to_string(value);
        }
    }

}   // namespace orm

#endif   // __ORM__FIELD_TPP__