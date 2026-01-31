#ifndef __ORM__FIELDS_DETAIL_HPP__
#define __ORM__FIELDS_DETAIL_HPP__

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

namespace orm::detail
{
    /**
     * @brief Convert a tuple of fields to an array of field views
     *
     * @tparam Tuple
     * @tparam Indices
     * @param tuple
     * @param indices
     * @return constexpr auto
     */
    template <typename Tuple, std::size_t... Indices>
    constexpr auto tuple_to_field_array(
        Tuple&& tuple,
        std::index_sequence<Indices...>
    );

}   // namespace orm::detail

#ifndef __ORM__FIELDS_DETAIL_TPP__
#include "fields_detail.tpp"
#endif   // __ORM__FIELDS_DETAIL_TPP__

#endif   // __ORM__FIELDS_DETAIL_HPP__