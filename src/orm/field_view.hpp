#ifndef __ORM__FIELD_VIEW_HPP__
#define __ORM__FIELD_VIEW_HPP__

#include <string>
#include <string_view>
#include <type_traits>

#include "db/statement.hpp"

namespace orm
{
    /**
     * @brief A view into a field of a database model
     *
     */
    class FieldView
    {
       private:
        using bind_function_type = void (*)(
            void const*    fieldAddress,
            db::Statement& statement,
            int            index
        );

        using read_function_type = void (*)(
            void*                fieldAddress,
            db::Statement const& statement,
            int                  col
        );

        using ddl_function_type = std::string (*)();

        void*       _mutableFieldAddress{nullptr};
        void const* _constFieldAddress{nullptr};

        std::string_view _columnName{};
        bool             _isPk{false};
        bool             _isAutoIncrement{false};
        bool             _isAutoIncrementPk{false};

        bind_function_type _bindFunction{nullptr};
        read_function_type _readFunction{nullptr};
        ddl_function_type  _ddlFunction{nullptr};

       public:
        FieldView() = default;

        [[nodiscard]] std::string_view getColumnName() const;
        [[nodiscard]] bool             isPk() const;
        [[nodiscard]] bool             isAutoIncrement() const;
        [[nodiscard]] bool             isAutoIncrementPk() const;

        void bind(db::Statement& statement, int index) const;
        void readFrom(db::Statement const& statement, int col) const;
        [[nodiscard]] std::string ddl() const;

        /**
         * @brief Create a FieldView from a field instance
         *
         * @tparam Field
         * @param field
         * @return FieldView
         */
        template <typename Field>
        static FieldView from(Field& field)
        {
            FieldView view{};
            view._mutableFieldAddress = static_cast<void*>(&field);
            view._constFieldAddress   = static_cast<void const*>(&field);

            view._columnName        = Field::name.view();
            view._isPk              = Field::isPk;
            view._isAutoIncrement   = Field::isAutoIncrement;
            view._isAutoIncrementPk = Field::isAutoIncrementPk;

            view._bindFunction = [](void const*    fieldAddress,
                                    db::Statement& statement,
                                    int            index)
            {
                auto const* typed_field =
                    static_cast<Field const*>(fieldAddress);

                typed_field->bind(statement, index);
            };

            view._readFunction =
                [](void* fieldAddress, db::Statement const& statement, int col)
            {
                auto* typed_field = static_cast<Field*>(fieldAddress);
                typed_field->readFrom(statement, col);
            };

            view._ddlFunction = []() -> std::string { return Field::ddl(); };

            return view;
        }

        /**
         * @brief Create a FieldView from a const field instance
         *
         * @tparam Field
         * @param field
         * @return FieldView
         */
        template <typename Field>
        static FieldView from(Field const& field)
        {
            FieldView view{};
            view._mutableFieldAddress = nullptr;
            view._constFieldAddress   = static_cast<void const*>(&field);

            view._columnName        = Field::name.view();
            view._isPk              = Field::isPk;
            view._isAutoIncrement   = Field::isAutoIncrement;
            view._isAutoIncrementPk = Field::isAutoIncrementPk;

            view._bindFunction = [](void const*    fieldAddress,
                                    db::Statement& statement,
                                    int            index)
            {
                auto const* typed_field =
                    static_cast<Field const*>(fieldAddress);

                typed_field->bind(statement, index);
            };

            view._readFunction = nullptr;   // cannot read into const field
            view._ddlFunction  = []() -> std::string { return Field::ddl(); };

            return view;
        }

       private:
        void _ensureBindable() const;
        void _ensureReadable() const;
        void _ensureHasDDL() const;
    };
}   // namespace orm

#endif   // __ORM__FIELD_VIEW_HPP__
