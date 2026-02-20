#ifndef __SETTINGS__PARAMS__PARAM_UTILS_HPP__
#define __SETTINGS__PARAMS__PARAM_UTILS_HPP__

#include <utility>

#include "nlohmann/json.hpp"

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

    /**
     * @brief Serialize a tuple of parameters to JSON
     *
     * @tparam Tuple
     * @param tuple
     * @return nlohmann::json
     */
    template <typename Tuple>
    constexpr nlohmann::json paramsToJson(Tuple&& tuple)
    {
        nlohmann::json jsonData;

        auto paramToJson = [&](const auto& param)
        { jsonData[param.getKey()] = param.toJson(); };

        forEachParam(std::forward<Tuple>(tuple), paramToJson);

        return jsonData;
    }

    /**
     * @brief Deserialize a tuple of parameters from JSON
     *
     * @tparam Tuple
     * @param tuple
     * @param jsonData
     */
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

#endif   // __SETTINGS__PARAMS__PARAM_UTILS_HPP__