#ifndef __ORM__FIELD_HPP__
#define __ORM__FIELD_HPP__

#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "orm/binder.hpp"
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
        Value _value{};

       public:
        using value_type                   = Value;
        static constexpr fixed_string name = Name;

        // clang-format off
        static constexpr bool isPk                = has_option_v<primary_key_t, Options...>;
        static constexpr bool isAutoIncrement    = has_option_v<auto_increment_t, Options...>;
        static constexpr bool isAutoIncrementPk = isPk && isAutoIncrement;
        static constexpr bool is_unique            = has_option_v<unique_t, Options...>;
        static constexpr bool is_nullable          = is_nullable_v<Value, Options...>;
        // clang-format on

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
    };

    template <typename Value>
    using IdField = Field<"id", Value, primary_key_t, auto_increment_t>;

}   // namespace orm

#ifndef __ORM__FIELD_TPP__
#include "orm/field.tpp"
#endif   // __ORM__FIELD_TPP__

#endif   // __ORM__FIELD_HPP__