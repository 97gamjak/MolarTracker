#include "general_settings.hpp"

#include "config/constants.hpp"

namespace settings
{
    /**
     * @brief Construct a new GeneralSettings:: GeneralSettings object
     *
     */
    GeneralSettings::GeneralSettings()
        : _core{
              Schema::GENERAL_SETTINGS_KEY,
              Schema::GENERAL_SETTINGS_TITLE,
              Schema::GENERAL_SETTINGS_DESCRIPTION
          }
    {
        _version.set(utils::SemVer(Constants::getVersion()));
        _currentVersion = _version.get();
    }

    /**
     * @brief Get the parameters of GeneralSettings as a tuple (const version)
     *
     * @return auto
     */
    auto GeneralSettings::_getParams() & { return std::tie(_version); }

    /**
     * @brief Get the parameters of GeneralSettings as a tuple (non-const
     * version)
     *
     * @return auto
     */
    auto GeneralSettings::_getParams() const& { return std::tie(_version); }

    /**
     * @brief Get the core parameter container of GeneralSettings
     *
     * @return ParamContainer&
     */
    ParamContainer& GeneralSettings::core() { return _core; }

    /**
     * @brief Get the core parameter container of GeneralSettings (const
     * version)
     *
     * @return const ParamContainer&
     */
    const ParamContainer& GeneralSettings::core() const { return _core; }

    /**
     * @brief Serialize GeneralSettings to JSON
     *
     * @return nlohmann::json
     */
    nlohmann::json GeneralSettings::toJson() const
    {
        return paramsToJson(_getParams());
    }

    /**
     * @brief Deserialize GeneralSettings from JSON
     *
     * @param jsonData
     * @param settings
     */
    void GeneralSettings::fromJson(
        const nlohmann::json& jsonData,
        GeneralSettings&      settings
    )
    {
        paramsFromJson(settings._getParams(), jsonData);
    }

}   // namespace settings