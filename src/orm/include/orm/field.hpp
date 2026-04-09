#ifndef __ORM__INCLUDE__ORM__FIELD_HPP__
#define __ORM__INCLUDE__ORM__FIELD_HPP__

#include <format>   // IWYU pragma: keep
#include <string>

#include "orm/concepts.hpp"
#include "orm/constraints.hpp"
#include "orm/fixed_string.hpp"
#include "orm/index.hpp"

namespace orm
{
    /**
     * @brief A field in a database model
     *
     * @tparam Name The name of the field
     * @tparam Value The type of the field's value
     * @tparam Options Additional constraints or options for the field
     */
    template <
        fixed_string Name,
        typename Value,
        fixed_string TableName,
        typename... Options>
    class Field
    {
       private:
        /// The value of the field
        Value _value{};

       public:
        static constexpr fixed_string tableName = TableName;

        /// Type alias for the field's value type
        using value_type = Value;

        /// The name of the field as a fixed string
        static constexpr fixed_string name = Name;

        /// Compile-time flag indicating whether this field is a primary key
        static constexpr bool isPk = has_option_v<primary_key_t, Options...>;

        /// Compile-time flag indicating whether this field is a foreign key
        static constexpr bool isFk = has_foreign_key_v<Value, Options...>;

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

        [[nodiscard]] std::string valueAsString() const;

        Field& operator=(Value value);

        [[nodiscard]] static std::string ddl();
        [[nodiscard]] static std::string getFkConstraints();

        template <typename Statement>
        void bind(Statement& statement, BindIndex index) const;

        template <typename Statement>
        void readFrom(Statement const& statement, ColumnIndex col);

        [[nodiscard]] static constexpr std::string   getFullColumnName();
        [[nodiscard]] static constexpr ORMConstraint getConstraints();

       private:
        /// Allow Field with different template parameters to access private
        /// members
        template <
            fixed_string NewName,
            typename NewValueType,
            fixed_string NewTableName,
            typename... NewOptions>
        friend class Field;

        [[nodiscard]] static std::string _valueAsString(Value value);
    };

    template <typename Value, fixed_string TableName>
    using IdField = Field<
        "id",
        Value,
        TableName,
        primary_key_t,
        auto_increment_t,
        unique_t>;

#define ORM_FIELD(Name, ...) \
    __VA_ARGS__ Name;        \
    using Name##Field = __VA_ARGS__;

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ORM_FIELDS(Self, ...)                                                \
    constexpr auto fields() { return std::tie(__VA_ARGS__); }                \
    constexpr auto fields() const { return std::tie(__VA_ARGS__); }          \
                                                                             \
    template <typename Func>                                                 \
    static void forEachColumn(Func&& func)                                   \
    {                                                                        \
        std::apply(                                                          \
            [&](auto... field) { (std::forward<Func>(func)(field), ...); },  \
            Self{}.fields()                                                  \
        );                                                                   \
    }                                                                        \
                                                                             \
    template <typename Func>                                                 \
    void forEachField(Func&& func)                                           \
    {                                                                        \
        std::apply(                                                          \
            [&](auto&... field) { (std::forward<Func>(func)(field), ...); }, \
            fields()                                                         \
        );                                                                   \
    }                                                                        \
                                                                             \
    template <typename Func>                                                 \
    void forEachField(Func&& func) const                                     \
    {                                                                        \
        std::apply(                                                          \
            [&](auto&... field) { (std::forward<Func>(func)(field), ...); }, \
            fields()                                                         \
        );                                                                   \
    }                                                                        \
                                                                             \
    [[nodiscard]] std::string toString() const                               \
    {                                                                        \
        std::string result = std::format("{} {{ ", tableName);               \
        bool        first  = true;                                           \
        forEachField(                                                        \
            [&](const auto& field)                                           \
            {                                                                \
                if (!first)                                                  \
                    result += ", ";                                          \
                first   = false;                                             \
                result += field.name + ": " + field.valueAsString();         \
            }                                                                \
        );                                                                   \
        result += " }";                                                      \
        return result;                                                       \
    }

}   // namespace orm

#ifndef __ORM__INCLUDE__ORM__FIELD_TPP__
#include "orm/field.tpp"
#endif   // __ORM__INCLUDE__ORM__FIELD_TPP__

#endif   // __ORM__INCLUDE__ORM__FIELD_HPP__