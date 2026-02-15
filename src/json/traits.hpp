#ifndef __JSON__TRAITS_HPP__
#define __JSON__TRAITS_HPP__

#include <nlohmann/json.hpp>
#include <type_traits>

namespace details
{
    /**
     * @brief  Trait to check if a type T is JSON serializable, it checks if the
     * type has the toJson() method and the fromJson(const nlohmann::json&, T&)
     * method, this is used to enable the to_json and from_json functions for
     * types that are JSON serializable
     *
     * @tparam T
     * @tparam typename
     */
    template <typename T, typename = void>
    struct is_json_serializable : std::false_type
    {
    };

    /**
     * @brief Specialization of the is_json_serializable trait for types that
     * have the toJson() method and the fromJson(const nlohmann::json&, T&)
     * method
     *
     * @tparam T
     */
    template <typename T>
    struct is_json_serializable<
        T,
        std::void_t<
            decltype(std::declval<T>().toJson()),
            decltype(T::fromJson(
                std::declval<const nlohmann::json&>(),
                std::declval<T&>()
            ))> > : std::true_type
    {
    };

}   // namespace details

/**
 * @brief Trait to check if a type T is JSON serializable, this is used to
 * enable the to_json and from_json functions for types that are JSON
 * serializable
 *
 * @tparam T
 */
template <typename T>
inline constexpr bool is_json_serializable_v =
    details::is_json_serializable<T>::value;

/**
 * @brief Concept to check if a type T is JSON serializable, this is used to
 * enable the to_json and from_json functions for types that are JSON
 * serializable
 *
 * @tparam T
 */
template <typename T>
concept JsonSerializable = is_json_serializable_v<T>;

/**
 * @brief to_json function for types that are JSON serializable, it calls the
 * toJson() method of the type T to serialize it to JSON
 *
 * @tparam T
 * @param j
 * @param obj
 */
template <JsonSerializable T>
void to_json(nlohmann::json& j, const T& obj)
{
    j = obj.toJson();
}

/**
 * @brief from_json function for types that are JSON serializable, it calls the
 * fromJson(const nlohmann::json&, T&) method of the type T to deserialize it
 * from JSON
 *
 * @tparam T
 * @param j
 * @param obj
 */
template <JsonSerializable T>
void from_json(const nlohmann::json& j, T& obj)
{
    T::fromJson(j, obj);
}

#endif   // __JSON__TRAITS_HPP__