#include "set_active_profile_command.hpp"

#include <format>

#include "app/store/profile_store.hpp"
#include "ui/commands/profile/add_profile_command_error.hpp"

#define __LOG_CATEGORY__ LogCategory::ui_AddProfileCommand
#include "logging/log_macros.hpp"

namespace ui
{
    SetActiveProfileCommand::SetActiveProfileCommand(
        std::string        profileName,
        app::ProfileStore& profileStore
    )
        : _profileName(std::move(profileName)), _profileStore(profileStore)
    {
    }

    std::expected<void, CommandErrorPtr> SetActiveProfileCommand::undo()
    {
        // We assume that everything is correct in the undo/redo process, so we
        // do not check if the previous profile exists, if it does not exist, we
        // just unset the active profile, which is a valid state for the
        // application, and it is better than having an error state because of a
        // missing profile.

        if (!_previousProfileName.has_value())
            _profileStore.unsetActiveProfile();
        else
            _profileStore.setActiveProfile(_previousProfileName->getName());

        return {};
    }

    std::expected<void, CommandErrorPtr> SetActiveProfileCommand::redo()
    {
        if (!_profileStore.profileExists(_profileName))
        {
            const auto errorMessage =
                std::format("Profile '{}' not found", _profileName);

            LOG_ERROR(errorMessage);

            std::unique_ptr<AddProfileCommandError> errorMessagePtr =
                std::make_unique<AddProfileCommandError>(
                    errorMessage,
                    AddProfileCommandErrorCode::ProfileNotFound
                );

            return std::unexpected<CommandErrorPtr>(std::move(errorMessagePtr));
        }

        _previousProfileName = _profileStore.getActiveProfile();

        _profileStore.setActiveProfile(_profileName);
        return {};
    }

    std::string SetActiveProfileCommand::getLabel() const
    {
        return "Set Active Profile: " + _profileName;
    }

}   // namespace ui
