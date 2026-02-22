#include "set_default_profile_command.hpp"

#include "settings/general_settings.hpp"

namespace ui
{
    /**
     * @brief Set the Default Profile Command:: Set Default Profile Command
     * object
     *
     * @param profileName
     * @param settings
     */
    SetDefaultProfileCommand::SetDefaultProfileCommand(
        std::string                profileName,
        settings::GeneralSettings& settings
    )
        : _profileName(std::move(profileName)), _settings(settings)
    {
    }

    /**
     * @brief Undo the set default profile command
     *
     * @return std::expected<void, CommandErrorPtr>
     */
    std::expected<void, CommandErrorPtr> SetDefaultProfileCommand::undo()
    {
        if (!_previousProfileName.has_value())
            _settings.unsetDefaultProfile();
        else
            _settings.setDefaultProfile(_previousProfileName.value());

        return {};
    }

    /**
     * @brief Redo the set default profile command
     *
     * @return std::expected<void, CommandErrorPtr>
     */
    std::expected<void, CommandErrorPtr> SetDefaultProfileCommand::redo()
    {
        _previousProfileName = _settings.getDefaultProfile();
        _settings.setDefaultProfile(_profileName.value());
        return {};
    }

    /**
     * @brief Get the Label object
     *
     * @return std::string
     */
    std::string SetDefaultProfileCommand::getLabel() const
    {
        return "Set Default Profile: " + _profileName.value();
    }

}   // namespace ui