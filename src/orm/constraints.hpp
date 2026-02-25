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

    struct primary_key_t
    {
        static constexpr ORMConstraint value = ORMConstraint::PrimaryKey;
    };

    struct auto_increment_t
    {
        static constexpr ORMConstraint value = ORMConstraint::AutoIncrement;
    };

    struct unique_t
    {
        static constexpr ORMConstraint value = ORMConstraint::Unique;
    };

    struct not_null_t
    {
        static constexpr ORMConstraint value = ORMConstraint::NotNull;
    };

    struct nullable_t
    {
        static constexpr ORMConstraint value = ORMConstraint::Nullable;
    };

    // clang-format on
}   // namespace orm

#endif   // __ORM__CONSTRAINTS_HPP__