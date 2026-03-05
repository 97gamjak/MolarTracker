#ifndef __ORM__FIELDS_HPP__
#define __ORM__FIELDS_HPP__

namespace orm
{
    template <typename Model>
    constexpr auto fields(Model& model);

    template <typename Model>
    constexpr auto fields(Model const& model);

}   // namespace orm

#ifndef __ORM__FIELDS_TPP__
#include "fields.tpp"   // IWYU pragma: export
#endif

#endif   // __ORM__FIELDS_HPP__