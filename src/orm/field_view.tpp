#ifndef __ORM__FIELD_VIEW_TPP__
#define __ORM__FIELD_VIEW_TPP__

#include "field_view.hpp"

namespace orm
{
    /**
     * @brief Create a FieldView from a field instance
     *
     * @tparam Field
     * @param field
     * @return FieldView
     */
    template <typename Field>
    FieldView FieldView::from(Field& field)
    {
        static_assert(
            Field::isAutoIncrement && !Field::isPk,
            "Auto-incrementing fields that are not primary keys are not "
            "supported!"
        );
        FieldView view{};
        view._mutableFieldAddress = static_cast<void*>(&field);
        view._constFieldAddress   = static_cast<void const*>(&field);

        view._columnName  = Field::name.view();
        view._constraints = Field::getConstraints();

        view._isPk              = Field::isPk;
        view._isAutoIncrement   = Field::isAutoIncrement;
        view._isAutoIncrementPk = Field::isAutoIncrementPk;

        view._bindFunction =
            [](void const* fieldAddress, db::Statement& statement, int index)
        {
            auto const* typed_field = static_cast<Field const*>(fieldAddress);

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
    FieldView FieldView::from(Field const& field)
    {
        static_assert(
            Field::isAutoIncrement && !Field::isPk,
            "Auto-incrementing fields that are not primary keys are not "
            "supported!"
        );
        FieldView view{};
        view._mutableFieldAddress = nullptr;
        view._constFieldAddress   = static_cast<void const*>(&field);

        view._columnName        = Field::name.view();
        view._constraints       = Field::getConstraints();
        view._isPk              = Field::isPk;
        view._isAutoIncrement   = Field::isAutoIncrement;
        view._isAutoIncrementPk = Field::isAutoIncrementPk;

        view._bindFunction =
            [](void const* fieldAddress, db::Statement& statement, int index)
        {
            auto const* typed_field = static_cast<Field const*>(fieldAddress);

            typed_field->bind(statement, index);
        };

        view._readFunction = nullptr;   // cannot read into const field
        view._ddlFunction  = []() -> std::string { return Field::ddl(); };

        return view;
    }
}   // namespace orm

#endif   // __ORM__FIELD_VIEW_TPP__