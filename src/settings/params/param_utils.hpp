#ifndef __SETTINGS__PARAMS_UTILS_HPP__
#define __SETTINGS__PARAMS_UTILS_HPP__

#include <utility>

#include "nlohmann/json.hpp"

template <class Derived, class T>
class ParamMixin;   // forward declaration

template <class Derived>
class ParamContainerMixin;   // forward declaration

namespace settings::detail
{
    template <class D, class T>
    std::true_type isParamMixinPtr(const ParamMixin<D, T>*);

    template <class D>
    std::true_type isParamContainerMixinPtr(const ParamContainerMixin<D>*);

    std::false_type isParamMixinPtr(...);
    std::false_type isParamContainerMixinPtr(...);

    template <class U>
    inline constexpr bool isParamMixin_v =
        decltype(isParamMixinPtr(std::declval<const std::remove_cvref_t<U>*>())
        )::value;

    template <class U>
    inline constexpr bool isParamContainerMixin_v =
        decltype(isParamContainerMixinPtr(
            std::declval<const std::remove_cvref_t<U>*>()
        ))::value;
}   // namespace settings::detail

namespace settings
{
    template <class U>
    concept ParamLike = settings::detail::isParamMixin_v<U> ||
                        settings::detail::isParamContainerMixin_v<U>;
}

namespace settings
{

    template <settings::ParamLike P>
    void to_json(nlohmann::json& j, const P& p)
    {
        j = p.toJson();
    }

    template <settings::ParamLike P>
    void from_json(const nlohmann::json& j, P& p)
    {
        // TODO: implement json concept for classes
        // https://97gamjak.atlassian.net/browse/MOLTRACK-139
        p.fromJson(j);
    }

}   // namespace settings

namespace settings
{
    /**
     * @brief Apply a function to each parameter in a tuple
     *
     * @tparam Tuple
     * @tparam Func
     * @tparam Is
     * @param tuple
     * @param func
     */
    template <typename Tuple, typename Func, std::size_t... Is>
    constexpr void forEachParamImpl(
        Tuple&& tuple,
        Func&&  func,
        std::index_sequence<Is...>
    )
    {
        (func(std::get<Is>(std::forward<Tuple>(tuple))), ...);
    }

    /**
     * @brief Apply a function to each parameter in a tuple
     *
     * @tparam Tuple
     * @tparam Func
     * @param tuple
     * @param func
     */
    template <typename Tuple, typename Func>
    constexpr void forEachParam(Tuple&& tuple, Func&& func)
    {
        constexpr auto size = std::tuple_size_v<std::remove_reference_t<Tuple>>;

        forEachParamImpl(
            std::forward<Tuple>(tuple),
            std::forward<Func>(func),
            std::make_index_sequence<size>{}
        );
    }

    template <typename Tuple>
    constexpr nlohmann::json paramsToJson(Tuple&& tuple)
    {
        nlohmann::json jsonData;

        auto paramToJson = [&](const auto& param)
        { jsonData[param.getKey()] = param.toJson(); };

        forEachParam(std::forward<Tuple>(tuple), paramToJson);

        return jsonData;
    }

    template <typename Tuple>
    constexpr void paramsFromJson(Tuple&& tuple, const nlohmann::json& jsonData)
    {
        auto paramFromJson = [&](auto& param)
        {
            const auto& key = param.getKey();

            if (jsonData.contains(key))
                std::remove_cvref_t<decltype(param)>::fromJson(jsonData, param);
        };

        forEachParam(std::forward<Tuple>(tuple), paramFromJson);
    }

}   // namespace settings

#endif   // __SETTINGS__PARAMS_UTILS_HPP__