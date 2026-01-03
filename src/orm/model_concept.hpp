#ifndef __ORM__MODEL_CONCEPTS_HPP__
#define __ORM__MODEL_CONCEPTS_HPP__

#include <concepts>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace orm
{
    template <typename T>
    concept tuple_like =
        requires { std::tuple_size<std::remove_reference_t<T>>::value; };

    template <typename T>
    concept db_model = requires(T instance) {
        { T::table_name.view() } -> std::convertible_to<std::string_view>;
        { instance.fields() } -> tuple_like;
        { std::as_const(instance).fields() } -> tuple_like;
    };
}   // namespace orm

#endif   // __ORM__MODEL_CONCEPTS_HPP__