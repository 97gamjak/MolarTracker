#include "field_view.hpp"

#include <string>
#include <string_view>

#include "db/statement.hpp"
#include "orm/orm_exception.hpp"

namespace orm
{
    /**
     * @brief Get the column name
     *
     * @return std::string_view
     */
    std::string_view FieldView::getColumnName() const { return _columnName; }

    /**
     * @brief Get the field constraints
     *
     * @return ORMConstraint
     */
    ORMConstraint FieldView::getConstraints() const { return _constraints; }

    /**
     * @brief Check if the field is a primary key
     *
     * @return true
     * @return false
     */
    bool FieldView::isPk() const { return _isPk; }

    /**
     * @brief Check if the field is auto-incrementing
     *
     * @return true
     * @return false
     */
    bool FieldView::isAutoIncrement() const { return _isAutoIncrement; }

    /**
     * @brief Check if the field is an auto-incrementing primary key
     *
     * @return true
     * @return false
     */
    bool FieldView::isAutoIncrementPk() const { return _isAutoIncrementPk; }

    /**
     * @brief Bind the field value to the specified parameter index
     *
     * @param statement
     * @param index
     */
    void FieldView::bind(db::Statement& statement, int index) const
    {
        _ensureBindable();
        _bindFunction(_constFieldAddress, statement, index);
    }

    /**
     * @brief Read the field value from the specified column
     *
     * @param statement
     * @param col
     */
    void FieldView::readFrom(db::Statement const& statement, int col) const
    {
        _ensureReadable();
        _readFunction(_mutableFieldAddress, statement, col);
    }

    /**
     * @brief Get the DDL string for the field
     *
     * @return std::string
     */
    std::string FieldView::ddl() const
    {
        _ensureHasDDL();
        return _ddlFunction();
    }

    //
    //
    // PRIVATE HELPER METHODS
    //
    //

    /**
     * @brief Ensure the field is bindable
     *
     */
    void FieldView::_ensureBindable() const
    {
        if (_constFieldAddress == nullptr || _bindFunction == nullptr)
            throw ORMError("FieldView is not bindable");
    }

    /**
     * @brief Ensure the field is readable
     *
     */
    void FieldView::_ensureReadable() const
    {
        if (_mutableFieldAddress == nullptr || _readFunction == nullptr)
            throw ORMError("FieldView is not readable");
    }

    /**
     * @brief Ensure the field has a DDL function
     *
     */
    void FieldView::_ensureHasDDL() const
    {
        if (_ddlFunction == nullptr)
            throw ORMError("FieldView has no ddl function");
    }

}   // namespace orm