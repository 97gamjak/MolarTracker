#include "field_view.hpp"

#include <string>
#include <string_view>
#include <type_traits>

#include "db/statement.hpp"
#include "orm/orm_exception.hpp"

namespace orm
{
    std::string_view FieldView::column_name() const noexcept
    {
        return _column_name;
    }

    bool FieldView::is_primary_key() const noexcept { return _is_primary_key; }

    bool FieldView::is_auto_increment() const noexcept
    {
        return _is_auto_increment;
    }

    bool FieldView::is_auto_increment_primary_key() const noexcept
    {
        return _is_auto_increment_primary_key;
    }

    void FieldView::bind(db::Statement& statement, int index) const
    {
        _ensure_bindable();
        _bind_function(_const_field_address, statement, index);
    }

    void FieldView::read_from(db::Statement const& statement, int col) const
    {
        _ensure_readable();
        _read_function(_mutable_field_address, statement, col);
    }

    std::string FieldView::ddl() const
    {
        _ensure_has_ddl();
        return _ddl_function();
    }

    //
    //
    // PRIVATE HELPER METHODS
    //
    //

    void FieldView::_ensure_bindable() const
    {
        if (_const_field_address == nullptr || _bind_function == nullptr)
        {
            throw ORMError("FieldView is not bindable");
        }
    }

    void FieldView::_ensure_readable() const
    {
        if (_mutable_field_address == nullptr || _read_function == nullptr)
        {
            throw ORMError("FieldView is not readable");
        }
    }

    void FieldView::_ensure_has_ddl() const
    {
        if (_ddl_function == nullptr)
        {
            throw ORMError("FieldView has no ddl function");
        }
    }

}   // namespace orm