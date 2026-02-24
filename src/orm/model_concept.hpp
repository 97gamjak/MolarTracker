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

    /**
     * @brief Concept for checking if a model has unique groups
     *
     * @tparam T
     */
    template <typename T>
    concept has_unique_groups = requires(T instance) {
        requires db_model<T>;
        { instance.getUniqueGroups() } -> tuple_like;
    };

    /**
     * @brief Represents a unique constraint group for a model
     *
     * @tparam Members
     */
    template <auto... Members>
    struct unique_group
    {
        /// A tuple of pointers to the fields that are part of this unique group
        static constexpr auto members = std::tuple{Members...};
    };

    /**
     * @brief Helper function to create a unique group
     *
     * @tparam Members
     * @return constexpr unique_group<Members...>
     */
    template <auto... Members>
    constexpr unique_group<Members...> unique()
    {
        return {};
    }

    /**
     * @brief Helper function to create a set of unique groups
     *
     * @tparam Groups
     * @return constexpr auto
     */
    template <typename... Groups>
    constexpr auto unique_set(Groups... groups)
    {
        return std::tuple{groups...};
    }
}   // namespace orm

#endif   // __ORM__MODEL_CONCEPT_HPP__