#ifndef __ORM__FIELDS_DETAIL_TPP__
#define __ORM__FIELDS_DETAIL_TPP__

#include <array>
#include <utility>

#include "orm/field_view.hpp"
#include "orm/fields_detail.hpp"

namespace orm::detail
{
    /**
     * @brief convert a tuple to an array of FieldView
     *
     * @tparam Tuple
     * @tparam Indices
     * @param tuple
     * @param index_sequence
     * @return constexpr auto
     */
    template <typename Tuple, std::size_t... Indices>
    constexpr auto tuple_to_field_array(
        Tuple&& tuple,
        std::index_sequence<Indices...> /*dummy*/
    )
    {
        return std::array<FieldView, sizeof...(Indices)>{
            FieldView::from(std::get<Indices>(std::forward<Tuple>(tuple)))...
        };
    }

}   // namespace orm::detail

#endif   // __ORM__FIELDS_DETAIL_TPP__