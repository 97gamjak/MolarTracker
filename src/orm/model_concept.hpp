#ifndef __ORM__MODEL_CONCEPT_HPP__
#define __ORM__MODEL_CONCEPT_HPP__

#include <concepts>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace orm
{
    /**
     * @brief Concept to check if a type is tuple-like
     *
     * @tparam T
     */
    template <typename T>
    concept tuple_like =
        requires { std::tuple_size<std::remove_reference_t<T>>::value; };

    /**
     * @brief Concept for database models
     *
     * @tparam T
     */
    template <typename T>
    concept db_model = requires(T instance) {
        { T::table_name } -> std::convertible_to<std::string>;
        { instance.fields() } -> tuple_like;
        { std::as_const(instance).fields() } -> tuple_like;
    };
}   // namespace orm

#endif   // __ORM__MODEL_CONCEPT_HPP__