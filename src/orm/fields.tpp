#ifndef __ORM__FIELDS_TPP__
#define __ORM__FIELDS_TPP__

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

#include "field_view.hpp"
#include "fields.hpp"
#include "fields_detail.hpp"

namespace orm
{
    // TODO(97gamjak): extract this to mstd
    // https://97gamjak.atlassian.net/browse/MSTD-84
    template <std::size_t N>
    using index_seq = std::make_index_sequence<N>;

    // TODO(97gamjak): extract this to mstd
    // https://97gamjak.atlassian.net/browse/MSTD-85
    template <typename T>
    using rm_ref_t = std::remove_reference_t<T>;

    /**
     * @brief get an array of FieldView from a model instance
     *
     * @tparam Model
     * @param model
     * @return constexpr auto
     */
    template <typename Model>
    constexpr auto fields(Model& model)
    {
        auto tuple                     = model.fields();
        using tuple_type               = rm_ref_t<decltype(tuple)>;
        constexpr std::size_t n_fields = std::tuple_size_v<tuple_type>;

        return detail::tuple_to_field_array(tuple, index_seq<n_fields>{});
    }

    /**
     * @brief get an array of FieldView from a const model instance
     *
     * @tparam Model
     * @param model
     * @return constexpr auto
     */
    template <typename Model>
    constexpr auto fields(Model const& model)
    {
        auto tuple                     = model.fields();
        using tuple_type               = rm_ref_t<decltype(tuple)>;
        constexpr std::size_t n_fields = std::tuple_size_v<tuple_type>;

        return detail::tuple_to_field_array(tuple, index_seq<n_fields>{});
    }

}   // namespace orm

#endif   // __ORM__FIELDS_TPP__