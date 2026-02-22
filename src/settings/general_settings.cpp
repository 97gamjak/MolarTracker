#include "general_settings.hpp"

#include "config/constants.hpp"

namespace settings
{
    /**
     * @brief Construct a new GeneralSettings:: GeneralSettings object
     *
     */
    GeneralSettings::GeneralSettings()
        : _core{Schema::GENERAL_SETTINGS_KEY, Schema::GENERAL_SETTINGS_TITLE, Schema::GENERAL_SETTINGS_DESCRIPTION},
          _currentVersion(utils::SemVer(Constants::getVersion()))
    {
        _version.set(utils::SemVer(Constants::getVersion()));
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
    const std::optional<std::string>& GeneralSettings::getDefaultProfile() const
    {
        return _defaultProfile.getOptional();
    }

    /**
     * @brief Check if a default profile name is set
     *
     * @return bool
     */
    bool GeneralSettings::hasDefaultProfile() const
    {
        return _defaultProfile.getOptional().has_value();
    }

}   // namespace settings