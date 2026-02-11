#include "log_viewer_settings.hpp"

namespace settings
{
    LogViewerSettings::LogViewerSettings()
    {
        _reloadIntervalSec.setDescription(Schema::RELOAD_INTERVAL_SEC_DESC);
        _reloadIntervalSec.setDefault(Schema::RELOAD_INTERVAL_SEC_DEFAULT);
        _reloadIntervalSec.setMinValue(Schema::RELOAD_INTERVAL_SEC_MIN);
        _reloadIntervalSec.setPrecision(Schema::RELOAD_INTERVAL_SEC_PRECISION);
    }

    auto LogViewerSettings::getParams() const&
    {
        return std::tie(_reloadIntervalSec);
    }

    auto LogViewerSettings::getParams() &
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
        nlohmann::json jsonData;
        jsonData[Schema::RELOAD_INTERVAL_SEC_KEY] = _reloadIntervalSec;
        return jsonData;
    }

    /**
     * @brief Deserialize LogViewerSettings from JSON
     *
     * @param j
     * @return settings::LogViewerSettings
     */
    LogViewerSettings LogViewerSettings::fromJson(const nlohmann::json& j)
    {
        LogViewerSettings settings;

        const auto key              = Schema::RELOAD_INTERVAL_SEC_KEY;
        settings._reloadIntervalSec = j[key].get<NumericParam<double>>();

        return settings;
    }
}   // namespace settings