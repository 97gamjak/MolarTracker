#ifndef __CONFIG__INCLUDE__CONFIG__TYPE_TRAITS_HPP__
#define __CONFIG__INCLUDE__CONFIG__TYPE_TRAITS_HPP__

#include <type_traits>
#include <variant>

// TODO (97gamjak)[MOLTRACK-205]: migrate this to mstd later on

template <typename T>
struct is_variant : std::false_type
{
};

template <typename... Ts>
struct is_variant<std::variant<Ts...>> : std::true_type
{
};

template <typename T>
inline constexpr bool is_variant_v = is_variant<T>::value;

#endif   // __CONFIG__INCLUDE__CONFIG__TYPE_TRAITS_HPP__