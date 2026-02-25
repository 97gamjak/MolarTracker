#ifndef __ORM__FIELD_VIEW_HPP__
#define __ORM__FIELD_VIEW_HPP__

#include <string>
#include <string_view>

#include "db/statement.hpp"
#include "orm/constraints.hpp"

namespace orm
{
    /**
     * @brief A view into a field of a database model
     *
     */
    class FieldView
    {
       private:
        using _field = void*;
        /// Type alias for the bind function pointer type, which takes a pointer
        /// to the field's value, a reference to a db::Statement, and an index,
        /// and returns void
        using bind_function_type = void (*)(
            void const*    fieldAddress,
            db::Statement& statement,
            int            index
        );

        /// Type alias for the read function pointer type, which takes a pointer
        /// to the field's value, a const reference to a db::Statement, and a
        /// column index, and returns void
        using read_function_type = void (*)(
            void*                fieldAddress,
            db::Statement const& statement,
            int                  col
        );

        /// Type alias for the DDL function pointer type, which takes no
        /// parameters and returns a std::string containing the DDL definition
        /// of the field
        using ddl_function_type = std::string (*)();

        /// Pointer to the mutable field value (if the field is non-const), or
        /// nullptr if the field is const
        void* _mutableFieldAddress{nullptr};

        /// Pointer to the const field value (always valid)
        void const* _constFieldAddress{nullptr};

        /// The name of the database column corresponding to this field
        std::string_view _columnName;

        /// flag indicating whether this field is a primary key
        bool _isPk{false};

        /// flag indicating whether this field is auto-incremented
        bool _isAutoIncrement{false};

        /// flag indicating whether this field is both a primary
        /// key and auto-incremented
        bool _isAutoIncrementPk{false};

        /// Function pointer for binding the field's value to a db::Statement
        bind_function_type _bindFunction{nullptr};

        /// Function pointer for reading the field's value from a db::Statement
        read_function_type _readFunction{nullptr};

        /// Function pointer for getting the DDL definition of the field
        ddl_function_type _ddlFunction{nullptr};

       public:
        FieldView() = default;

        [[nodiscard]] std::string_view getColumnName() const;
        [[nodiscard]] bool             isPk() const;
        [[nodiscard]] bool             isAutoIncrement() const;
        [[nodiscard]] bool             isAutoIncrementPk() const;

        void bind(db::Statement& statement, int index) const;
        void readFrom(db::Statement const& statement, int col) const;
        [[nodiscard]] std::string ddl() const;

        template <typename Field>
        static FieldView from(Field& field);

        template <typename Field>
        static FieldView from(Field const& field);

        static consteval ORMConstraint getConstraints();

       private:
        void _ensureBindable() const;
        void _ensureReadable() const;
        void _ensureHasDDL() const;
    };
}   // namespace orm

#ifndef __ORM__FIELD_VIEW_TPP__
#include "field_view.tpp"
#endif   // __ORM__FIELD_VIEW_TPP__

#endif   // __ORM__FIELD_VIEW_HPP__