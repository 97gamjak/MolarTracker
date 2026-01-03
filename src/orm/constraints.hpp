#ifndef __ORM__CONSTRAINTS_HPP__
#define __ORM__CONSTRAINTS_HPP__

namespace orm
{
    // clang-format off
    
    struct primary_key_t {};
    struct auto_increment_t {};
    struct unique_t {};
    struct not_null_t {};
    struct nullable_t {};

    // clang-format on
}   // namespace orm

#endif   // __ORM__CONSTRAINTS_HPP__