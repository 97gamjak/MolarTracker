#include "log_viewer_settings.hpp"

#include "param_utils.hpp"

namespace settings
{
    /**
     * @brief Construct a new LogViewerSettings::LogViewerSettings object
     *
     */
    LogViewerSettings::LogViewerSettings()
        : ParamContainer(
              Schema::LOG_VIEWER_SETTINGS_KEY,
              Schema::LOG_VIEWER_SETTINGS_TITLE,
              Schema::LOG_VIEWER_SETTINGS_DESC
          )
    {
        _reloadIntervalSec.setDescription(Schema::RELOAD_INTERVAL_SEC_DESC);
        _reloadIntervalSec.setDefault(Schema::RELOAD_INTERVAL_SEC_DEFAULT);
        _reloadIntervalSec.setMinValue(Schema::RELOAD_INTERVAL_SEC_MIN);
        _reloadIntervalSec.setPrecision(Schema::RELOAD_INTERVAL_SEC_PRECISION);
    }

    /**
     * @brief Get the parameters of LogViewerSettings as a tuple (const version)
     *
     * @return auto
     */
    auto LogViewerSettings::_getParams() const&
    {
        return std::tie(_reloadIntervalSec);
    }

    /**
     * @brief Get the parameters of LogViewerSettings as a tuple (non-const
     * version)
     *
     * @return auto
     */
    auto LogViewerSettings::_getParams() &
    {
        return std::tie(_reloadIntervalSec);
    }

    /**
     * @brief Serialize LogViewerSettings to JSON
     *
     * @return nlohmann::json
     */
    nlohmann::json LogViewerSettings::toJson() const
    {
        return paramsToJson(_getParams());
    }

    /**
     * @brief Deserialize LogViewerSettings from JSON
     *
     * @param j
     * @return settings::LogViewerSettings
     */
    LogViewerSettings LogViewerSettings::fromJson(
        const nlohmann::json& jsonData
    )
    {
        LogViewerSettings settings;

        paramsFromJson(settings._getParams(), jsonData);

        return settings;
    }
}   // namespace settings