#ifndef __ORM__FIELD_VIEW_HPP__
#define __ORM__FIELD_VIEW_HPP__

#include <string>
#include <string_view>
#include <type_traits>

#include "db/statement.hpp"

namespace orm
{
    class FieldView
    {
       private:
        using bind_function_type = void (*)(
            void const*    field_address,
            db::Statement& statement,
            int            index
        );

        using read_function_type = void (*)(
            void*                field_address,
            db::Statement const& statement,
            int                  col
        );

        using ddl_function_type = std::string (*)();

        void*       _mutable_field_address{nullptr};
        void const* _const_field_address{nullptr};

        std::string_view _column_name{};
        bool             _is_pk{false};
        bool             _is_auto_increment{false};
        bool             _is_auto_increment_pk{false};

        bind_function_type _bind_function{nullptr};
        read_function_type _read_function{nullptr};
        ddl_function_type  _ddl_function{nullptr};

       public:
        FieldView() = default;

        [[nodiscard]] std::string_view column_name() const noexcept;
        [[nodiscard]] bool             is_pk() const noexcept;
        [[nodiscard]] bool             is_auto_increment() const noexcept;
        [[nodiscard]] bool             is_auto_increment_pk() const noexcept;

        void bind(db::Statement& statement, int index) const;
        void read_from(db::Statement const& statement, int col) const;
        [[nodiscard]] std::string ddl() const;

        template <typename Field>
        static FieldView from(Field& field)
        {
            FieldView view{};
            view._mutable_field_address = static_cast<void*>(&field);
            view._const_field_address   = static_cast<void const*>(&field);

            view._column_name          = Field::name.view();
            view._is_pk                = Field::is_pk;
            view._is_auto_increment    = Field::is_auto_increment;
            view._is_auto_increment_pk = Field::is_auto_increment_pk;

            view._bind_function = [](void const*    field_address,
                                     db::Statement& statement,
                                     int            index)
            {
                auto const* typed_field =
                    static_cast<Field const*>(field_address);
                typed_field->bind(statement, index);
            };

            view._read_function =
                [](void* field_address, db::Statement const& statement, int col)
            {
                auto* typed_field = static_cast<Field*>(field_address);
                typed_field->read_from(statement, col);
            };

            view._ddl_function = []() -> std::string { return Field::ddl(); };

            return view;
        }

        template <typename Field>
        static FieldView from(Field const& field)
        {
            FieldView view{};
            view._mutable_field_address = nullptr;
            view._const_field_address   = static_cast<void const*>(&field);

            view._column_name          = Field::name.view();
            view._is_pk                = Field::is_pk;
            view._is_auto_increment    = Field::is_auto_increment;
            view._is_auto_increment_pk = Field::is_auto_increment_pk;

            view._bind_function = [](void const*    field_address,
                                     db::Statement& statement,
                                     int            index)
            {
                auto const* typed_field =
                    static_cast<Field const*>(field_address);
                typed_field->bind(statement, index);
            };

            view._read_function = nullptr;   // cannot read into const field
            view._ddl_function  = []() -> std::string { return Field::ddl(); };

            return view;
        }

       private:
        void _ensure_bindable() const;
        void _ensure_readable() const;
        void _ensure_has_ddl() const;
    };
}   // namespace orm

#endif   // __ORM__FIELD_VIEW_HPP__
