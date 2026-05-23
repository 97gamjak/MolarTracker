#include "commands/profile/set_active_profile_command.hpp"

#include <expected>
#include <format>
#include <memory>
#include <utility>

#include "app/store/i_profile_store.hpp"
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
        std::string                                profileName,
        const std::shared_ptr<app::IProfileStore>& profileStore
    )
        : _profileName(std::move(profileName)), _profileStore(profileStore)
    {
    }

    /**
     * @brief Undo the Set Active Profile Command
     *
     * @return std::expected<void, CommandErrorPtr>
     */
    std::expected<void, CommandErrorPtr> SetActiveProfileCommand::undo()
    {
        const std::string baseMsg = "Failed to undo set active profile: ";

        if (!_previousProfile.has_value())
        {
            const auto errorMessage = baseMsg + "No previous active profile";

            LOG_ERROR(errorMessage);

            std::unique_ptr<AddProfileCommandError> errorMessagePtr =
                std::make_unique<AddProfileCommandError>(
                    errorMessage,
                    AddProfileCommandErrorCode::ProfileNotFound
                );

            return std::unexpected<CommandErrorPtr>(std::move(errorMessagePtr));
        }

        const auto result =
            _profileStore->setActiveProfile(_previousProfile->getName());

        if (result != app::ProfileStoreResult::Ok)
        {
            const auto errorMessage = baseMsg + _previousProfile->getName();

            LOG_ERROR(errorMessage);

            return std::unexpected<CommandErrorPtr>(
                std::make_unique<AddProfileCommandError>(
                    errorMessage,
                    AddProfileCommandErrorCode::ProfileNotFound
                )
            );
        }

        return {};
    }

    /**
     * @brief Redo the Set Active Profile Command
     *
     * @return std::expected<void, CommandErrorPtr>
     */
    std::expected<void, CommandErrorPtr> SetActiveProfileCommand::redo()
    {
        const auto* baseMsg = "Failed to redo set active profile: ";

        if (!_profileStore->profileExists(_profileName))
        {
            const auto errorMessage =
                baseMsg + std::format("Profile '{}' not found", _profileName);

            LOG_ERROR(errorMessage);

            std::unique_ptr<AddProfileCommandError> errorMessagePtr =
                std::make_unique<AddProfileCommandError>(
                    errorMessage,
                    AddProfileCommandErrorCode::ProfileNotFound
                );

            return std::unexpected<CommandErrorPtr>(std::move(errorMessagePtr));
        }

        _previousProfile = _profileStore->getActiveProfile();

        const auto result = _profileStore->setActiveProfile(_profileName);

        if (result != app::ProfileStoreResult::Ok)
        {
            LOG_ERROR(
                baseMsg + std::format("Profile '{}' not found", _profileName)
            );

            std::unique_ptr<AddProfileCommandError> errorMessagePtr =
                std::make_unique<AddProfileCommandError>(
                    std::format(
                        "Failed to redo set active profile: {}",
                        _profileName
                    ),
                    AddProfileCommandErrorCode::ProfileNotFound
                );

            return std::unexpected<CommandErrorPtr>(std::move(errorMessagePtr));
        }

        return {};
    }

    std::string SetActiveProfileCommand::getLabel() const
    {
        return "Set Active Profile: " + _profileName;
    }

}   // namespace cmd
