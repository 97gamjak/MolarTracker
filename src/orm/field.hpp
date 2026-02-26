#ifndef __ORM__FIELD_HPP__
#define __ORM__FIELD_HPP__

#include <string>

#include "orm/concepts.hpp"
#include "orm/constraints.hpp"
#include "orm/fixed_string.hpp"

namespace orm
{
    /**
     * @brief A field in a database model
     *
     * @tparam Name The name of the field
     * @tparam Value The type of the field's value
     * @tparam Options Additional constraints or options for the field
     */
    template <fixed_string Name, typename Value, typename... Options>
    class Field
    {
       private:
        /// The value of the field
        Value _value{};

       public:
        /// Type alias for the field's value type
        using value_type = Value;

        /// The name of the field as a fixed string
        static constexpr fixed_string name = Name;

        /// Compile-time flag indicating whether this field is a primary key
        static constexpr bool isPk = has_option_v<primary_key_t, Options...>;

        /// Compile-time flag indicating whether this field is auto-incremented
        static constexpr bool isAutoIncrement =
            has_option_v<auto_increment_t, Options...>;

        /// Compile-time flag indicating whether this field is a unique key
        static constexpr bool isAutoIncrementPk = isPk && isAutoIncrement;

        /// Compile-time flag indicating whether this field is unique
        static constexpr bool isUnique = has_option_v<unique_t, Options...>;

        /// Compile-time flag indicating whether this field is nullable
        static constexpr bool isNullable = is_nullable_v<Value, Options...>;

       public:
        Field() = default;
        explicit Field(Value value);

        [[nodiscard]] Value&       value();
        [[nodiscard]] Value const& value() const;

        Field& operator=(Value value);

        [[nodiscard]] static std::string ddl();

        template <typename Statement>
        void bind(Statement& statement, int index) const;

        template <typename Statement>
        void readFrom(Statement const& statement, int col);

        [[nodiscard]] static constexpr auto          getColumnName();
        [[nodiscard]] static constexpr ORMConstraint getConstraints();
    };

    template <typename Value>
    using IdField = Field<"id", Value, primary_key_t, auto_increment_t>;

}   // namespace orm

#ifndef __ORM__FIELD_TPP__
#include "orm/field.tpp"
#endif   // __ORM__FIELD_TPP__

#endif   // __ORM__FIELD_HPP__