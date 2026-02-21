#include "set_default_profile_command.hpp"

#include "settings/general_settings.hpp"

namespace ui
{
    SetDefaultProfileCommand::SetDefaultProfileCommand(
        std::string                profileName,
        settings::GeneralSettings& settings
    )
        : _profileName(std::move(profileName)), _settings(settings)
    {
    }

    std::expected<void, CommandErrorPtr> SetDefaultProfileCommand::undo()
    {
        if (!_previousProfileName.has_value())
            _settings.unsetDefaultProfile();
        else
            _settings.setDefaultProfile(_previousProfileName.value());

        return {};
    }

    std::expected<void, CommandErrorPtr> SetDefaultProfileCommand::redo()
    {
        _previousProfileName = _settings.getDefaultProfile();
        _settings.setDefaultProfile(_profileName.value());
        return {};
    }

    std::string SetDefaultProfileCommand::getLabel() const
    {
        return "Set Default Profile: " + _profileName.value();
    }

}   // namespace ui