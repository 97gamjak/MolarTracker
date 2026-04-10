#ifndef __ORM__INCLUDE__ORM__ORM_MODEL_HPP__
#define __ORM__INCLUDE__ORM__ORM_MODEL_HPP__

#include "field.hpp"
#include "fixed_string.hpp"

namespace orm
{
    /**
     * @brief Base class for ORM models, representing a database table
     *
     * @tparam TableName The name of the database table associated with this
     * model
     */
    template <fixed_string TableName>
    struct ORMModel
    {
        /// The name of the database table associated with this model, stored as
        /// a compile-time fixed string
        static constexpr fixed_string tableName = TableName;

        /// Helper type aliases for defining fields in the model, allowing for
        /// concise field definitions that automatically include the table name
        template <fixed_string FieldName, typename Value, typename... Options>
        using Field = Field<FieldName, Value, tableName, Options...>;

        /// Helper type alias for defining ID fields in the model, which are a
        /// special case of regular fields with additional constraints (e.g.,
        /// primary key, auto-increment, etc.)
        template <typename Value>
        using IdField = IdField<Value, tableName>;
    };
}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__ORM_MODEL_HPP__
