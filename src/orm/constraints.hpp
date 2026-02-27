#ifndef __ORM__CONSTRAINTS_HPP__
#define __ORM__CONSTRAINTS_HPP__

#include <cstdint>

#include "mstd/enum.hpp"

namespace orm
{

    // NOLINTBEGIN
#define ORM_CONSTRAINT_LIST(X) \
    X(PrimaryKey, 0x01)        \
    X(AutoIncrement, 0x02)     \
    X(Unique, 0x04)            \
    X(NotNull, 0x08)           \
    X(Nullable, 0x10)

    MSTD_ENUM_BITFLAG(ORMConstraint, uint64_t, ORM_CONSTRAINT_LIST);
    // NOLINTEND

    /**
     * @brief struct representing the primary key constraint for a field
     *
     */
    struct primary_key_t
    {
        /// Compile-time constant representing the primary key constraint
        static constexpr ORMConstraint value = ORMConstraint::PrimaryKey;
    };

    /**
     * @brief struct representing the auto-increment constraint for a field
     *
     */
    struct auto_increment_t
    {
        /// Compile-time constant representing the auto-increment constraint
        static constexpr ORMConstraint value = ORMConstraint::AutoIncrement;
    };

    /**
     * @brief struct representing the unique constraint for a field
     *
     */
    struct unique_t
    {
        /// Compile-time constant representing the unique constraint
        static constexpr ORMConstraint value = ORMConstraint::Unique;
    };

    /**
     * @brief struct representing the not-null constraint for a field
     *
     */
    struct not_null_t
    {
        /// Compile-time constant representing the not-null constraint
        static constexpr ORMConstraint value = ORMConstraint::NotNull;
    };

    /**
     * @brief struct representing the nullable constraint for a field
     *
     */
    struct nullable_t
    {
        /// Compile-time constant representing the nullable constraint
        static constexpr ORMConstraint value = ORMConstraint::Nullable;
    };

    // clang-format on
}   // namespace orm

#endif   // __ORM__CONSTRAINTS_HPP__