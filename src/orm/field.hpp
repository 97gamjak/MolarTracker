#ifndef __ORM__FIELD_HPP__
#define __ORM__FIELD_HPP__

#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "binder.hpp"
#include "concepts.hpp"
#include "constraints.hpp"
#include "fixed_string.hpp"
#include "sqlite_type.hpp"

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
        using value_type           = Value;
        static constexpr auto name = Name;

        // clang-format off
        static constexpr bool is_primary_key    = has_option_v<primary_key_t, Options...>;
        static constexpr bool is_auto_increment = has_option_v<auto_increment_t, Options...>;
        static constexpr bool is_unique         = has_option_v<unique_t, Options...>;
        static constexpr bool is_nullable       = is_nullable_v<Value, Options...>;
        // clang-format on

        Field() = default;
        explicit Field(Value value);

        [[nodiscard]] Value&       value() noexcept;
        [[nodiscard]] Value const& value() const noexcept;

        Field& operator=(Value value);

        [[nodiscard]] static std::string ddl();

        template <typename StatementLike>
        void bind(StatementLike& statement, int index) const;

        template <typename StatementLike>
        void read_from(StatementLike const& statement, int col);
    };
}   // namespace orm

#ifndef __ORM__FIELD_TPP__
#include "field.tpp"
#endif   // __ORM__FIELD_TPP__

#endif   // __ORM__FIELD_HPP__