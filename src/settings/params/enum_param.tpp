#ifndef __SETTINGS__ENUM_PARAM_TPP__
#define __SETTINGS__ENUM_PARAM_TPP__

#include "enum_param.hpp"
#include "nlohmann/json.hpp"
#include "param_error.hpp"

namespace settings
{
    /**
     * @brief Construct a new Enum Param:: Enum Param object
     *
     * @tparam E
     * @param key
     * @param title
     * @param description
     */
    template <typename E>
    requires std::is_enum_v<E>
    EnumParam<E>::EnumParam(
        std::string key,
        std::string title,
        std::string description
    )
        : _core(key, title, description)
    {
    }

    /**
     * @brief Serialize EnumParam to JSON
     *
     * @tparam E
     * @return nlohmann::json
     */
    template <typename E>
    requires std::is_enum_v<E>
    nlohmann::json EnumParam<E>::toJson() const
    {
        auto jsonData = _core.toJson();

        return jsonData;
    }

    /**
     * @brief Deserialize EnumParam from JSON
     *
     * @tparam E
     * @param jsonData
     * @param param
     */
    template <typename E>
    requires std::is_enum_v<E>
    void EnumParam<E>::fromJson(
        const nlohmann::json& jsonData,
        EnumParam<E>&         param
    )
    {
        ParamCore<E>::fromJson(jsonData, param._core);
    }

    /**
     * @brief Set the value of the enum parameter
     *
     * @tparam Derived
     * @tparam T
     * @param value
     * @return std::expected<void, ParamError>
     */
    template <typename E>
    requires std::is_enum_v<E>
    std::expected<void, ParamError> EnumParam<E>::set(const E& value)
    {
        for (const auto& validValue : EnumMeta::values_view())
        {
            if (validValue == value)
            {
                _core.set(value);
                return {};
            }
        }

        const auto errorMessage =
            "Invalid value for enum parameter: " +
            std::to_string(static_cast<std::underlying_type_t<E>>(value));

        return std::unexpected(ParamError{errorMessage});
    }

}   // namespace settings

#endif   // __SETTINGS__ENUM_PARAM_TPP__