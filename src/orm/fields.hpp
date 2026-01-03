#ifndef __ORM__FIELDS_HPP__
#define __ORM__FIELDS_HPP__

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

#include "field_view.hpp"

namespace orm
{
    template <typename Model>
    constexpr auto fields(Model& model);

    template <typename Model>
    constexpr auto fields(Model const& model);

}   // namespace orm

#ifndef __ORM__FIELDS_TPP__
#include "fields.tpp"
#endif   // __ORM__FIELDS_TPP__

#endif   // __ORM__FIELDS_HPP__