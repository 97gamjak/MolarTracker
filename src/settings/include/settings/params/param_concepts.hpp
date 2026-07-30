#ifndef __SETTINGS__INCLUDE__SETTINGS__PARAMS__PARAM_CONCEPTS_HPP__
#define __SETTINGS__INCLUDE__SETTINGS__PARAMS__PARAM_CONCEPTS_HPP__

#include <concepts>
#include <string>

namespace settings
{
    /**
     * @brief Concept for types that have a forEachParam function, this is used
     * to constrain the types that can be used with the paramsToJson and
     * paramsFromJson functions, this ensures that only types that have a
     * forEachParam function can be serialized and deserialized using these
     * functions
     *
     * @tparam T
     */
    template <class T>
    concept HasForEachParam =
        (requires(T& param) { param.forEachParam([](auto&&) {}); }) &&
        requires(const T& param) { param.forEachParam([](auto&&) {}); };

    /**
     * @brief Concept for parameter containers
     *
     * @tparam T
     */
    template <typename T>
    concept IsParamContainer = HasForEachParam<T>;

    /**
     * @brief Concept for parameter types, this is used to constrain the types
     * that can be used with the paramsToJson and paramsFromJson functions, this
     * ensures that only types that have the required member functions can be
     * serialized and deserialized using these functions
     *
     * @tparam T
     */
    template <typename T>
    concept IsParam = requires(T& param) {
        { param.getKey() } -> std::convertible_to<std::string>;
        { param.getTitle() } -> std::convertible_to<std::string>;
        { param.getDescription() } -> std::convertible_to<std::string>;
        { param.get() } -> std::convertible_to<typename T::value_type>;
        {
            param.set(std::declval<typename T::value_type>())
        } -> std::same_as<void>;
        { param.resetToDefault() } -> std::same_as<void>;
    };

}   // namespace settings

#endif   // __SETTINGS__INCLUDE__SETTINGS__PARAMS__PARAM_CONCEPTS_HPP__