#include "ui_settings.hpp"

#include "params/params.hpp"

namespace settings
{
    /**
     * @brief Construct a new UISettings::UISettings object
     *
     */
    UISettings::UISettings()
        : ParamContainer(
              Schema::UI_SETTINGS_KEY,
              Schema::UI_SETTINGS_TITLE,
              Schema::UI_SETTINGS_DESC
          )
    {
    }

    /**
     * @brief Get the parameters of UISettings as a tuple (const version)
     *
     * @return auto
     */
    auto UISettings::_getParams() const&
    {
        return std::tie(_logViewerSettings);
    }

    /**
     * @brief Get the parameters of UISettings as a tuple (non-const version)
     *
     * @return auto
     */
    auto UISettings::_getParams() & { return std::tie(_logViewerSettings); }

    /**
     * @brief Serialize UISettings to JSON
     *
     */
    nlohmann::json UISettings::toJson() const
    {
        return paramsToJson(_getParams());
    }

    /**
     * @brief Deserialize UISettings from JSON
     *
     * @param jsonData
     * @param settings
     */
    void UISettings::fromJson(
        const nlohmann::json& jsonData,
        UISettings&           settings
    )
    {
        paramsFromJson(settings._getParams(), jsonData);
    }
}   // namespace settings