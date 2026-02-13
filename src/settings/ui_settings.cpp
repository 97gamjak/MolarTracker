#include "ui_settings.hpp"

#include "param_utils.hpp"

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
     * @param j
     * @param settings
     */
    nlohmann::json UISettings::toJson() const
    {
        return paramsToJson(_getParams());
    }

    /**
     * @brief Deserialize UISettings from JSON
     *
     * @param j
     * @return settings::UISettings
     */
    UISettings UISettings::fromJson(const nlohmann::json& jsonData)
    {
        UISettings settings;

        paramsFromJson(settings._getParams(), jsonData);

        return settings;
    }
}   // namespace settings