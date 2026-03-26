#include "commands/profile/set_active_profile_command.hpp"

#include <expected>
#include <format>
#include <memory>
#include <utility>

#include "app/store/profile/profile_store.hpp"
#include "commands/profile/add_profile_command_error.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("Commands.SetActiveProfileCommand");

namespace cmd
{
    /**
     * @brief Set the Active Profile Command:: Set Active Profile Command object
     *
     * @param profileName
     * @param profileStore
     */
    SetActiveProfileCommand::SetActiveProfileCommand(
        std::string        profileName,
        app::ProfileStore& profileStore
    )
        : _profileName(std::move(profileName)), _profileStore(profileStore)
    {
    }

    std::expected<void, CommandErrorPtr> SetActiveProfileCommand::undo()
    {
        if (!_previousProfile.has_value())
        {
            const auto* const errorMessage =
                "Failed to undo set active profile: No previous active profile";

            LOG_ERROR(errorMessage);

            std::unique_ptr<AddProfileCommandError> errorMessagePtr =
                std::make_unique<AddProfileCommandError>(
                    errorMessage,
                    AddProfileCommandErrorCode::ProfileNotFound
                );

            return std::unexpected<CommandErrorPtr>(std::move(errorMessagePtr));
        }

        _profileStore.setActiveProfile(_previousProfile->getName());

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

        _previousProfile = _profileStore.getActiveProfile();

        _profileStore.setActiveProfile(_profileName);
        return {};
    }

    std::string SetActiveProfileCommand::getLabel() const
    {
        return "Set Active Profile: " + _profileName;
    }

}   // namespace cmd
