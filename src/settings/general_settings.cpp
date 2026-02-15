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
     * @brief Get the parameters of GeneralSettings as a tuple (non-const
     * version)
     *
     * @return auto
     */
    auto GeneralSettings::_getParams() &
    {
        return std::tie(_version, _defaultProfile);
    }

    /**
     * @brief Get the parameters of GeneralSettings as a tuple (const
     * version)
     *
     * @return auto
     */
    auto GeneralSettings::_getParams() const&
    {
        return std::tie(_version, _defaultProfile);
    }

    /**
     * @brief Set the default profile name
     *
     * @param profileName
     */
    void GeneralSettings::setDefaultProfile(const std::string& profileName)
    {
        _defaultProfile.set(profileName);
    }

    /**
     * @brief Unset the default profile name
     *
     */
    void GeneralSettings::unsetDefaultProfile() { _defaultProfile.unset(); }

    /**
     * @brief Get the default profile name
     *
     * @return std::optional<std::string>
     */
    std::optional<std::string> GeneralSettings::getDefaultProfile() const
    {
        return _defaultProfile.get();
    }

    /**
     * @brief Check if a default profile name is set
     *
     * @return bool
     */
    bool GeneralSettings::hasDefaultProfile() const
    {
        return _defaultProfile.get().has_value();
    }

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
        settings._savedVersion = settings._version.get();
    }

}   // namespace settings