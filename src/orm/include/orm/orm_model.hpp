#ifndef __ORM__INCLUDE__ORM__ORM_MODEL_HPP__
#define __ORM__INCLUDE__ORM__ORM_MODEL_HPP__

#include "field.hpp"
#include "fixed_string.hpp"

namespace orm
{
    template <fixed_string TableName>
    struct ORMModel
    {
        static constexpr fixed_string tableName = TableName;

        template <fixed_string FieldName, typename Value, typename... Options>
        using Field = Field<FieldName, Value, tableName, Options...>;

        template <typename Value>
        using IdField = IdField<Value, tableName>;
    };
}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__ORM_MODEL_HPP__
