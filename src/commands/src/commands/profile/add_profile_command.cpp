#include "commands/profile/add_profile_command.hpp"

#include <expected>
#include <format>
#include <memory>
#include <utility>

#include "commands/profile/add_profile_command_error.hpp"
#include "domain/profile.hpp"
#include "logging/log_macros.hpp"
#include "store/i_profile_store.hpp"

REGISTER_LOG_CATEGORY("Commands.Profile.AddProfileCommand");

namespace cmd
{
    /**
     * @brief Construct a new Add Profile Command:: Add Profile Command object
     *
     * @param profileStore
     * @param profile
     */
    AddProfileCommand::AddProfileCommand(
        const std::shared_ptr<store::IProfileStore>& profileStore,
        domain::Profile                              profile
    )
        : _profileStore{profileStore}, _profile{std::move(profile)}
    {
    }

    /**
     * @brief Undo the add profile command
     *
     * @return std::expected<void, CommandErrorPtr> Result of the operation
     *
     */
    std::expected<void, CommandErrorPtr> AddProfileCommand::undo()
    {
        const auto result = _profileStore->removeProfile(_profile);

        if (result == store::ProfileStoreResult::ProfileNotFound)
        {
            const auto errorMessage = std::format(
                "Failed to undo add profile: '{}' not found",
                _profile.getName()
            );

            LOG_ERROR(errorMessage);

            std::unique_ptr<AddProfileCommandError> error =
                std::make_unique<AddProfileCommandError>(
                    errorMessage,
                    AddProfileCommandErrorCode::ProfileNotFound
                );

            return std::unexpected<CommandErrorPtr>(std::move(error));
        }

        if (result == store::ProfileStoreResult::Ok)
        {
            LOG_INFO(std::format("Profile removed: '{}'", _profile.getName()));
        }
        else
        {
            const auto errorMessage = std::format(
                "Unknown error removing profile '{}'",
                _profile.getName()
            );

            LOG_ERROR(errorMessage);

            std::unique_ptr<AddProfileCommandError> error =
                std::make_unique<AddProfileCommandError>(
                    errorMessage,
                    AddProfileCommandErrorCode::UnknownError
                );

            return std::unexpected<CommandErrorPtr>(std::move(error));
        }

        return {};
    }

    /**
     * @brief Redo the add profile command
     *
     * @return std::expected<void, CommandErrorPtr> Result of the operation
     */
    std::expected<void, CommandErrorPtr> AddProfileCommand::redo()
    {
        const auto result = _profileStore->addProfile(_profile);

        if (result == store::ProfileStoreResult::Ok)
        {
            LOG_INFO(std::format("Profile created: '{}'", _profile.getName()));
        }
        else if (result == store::ProfileStoreResult::NameAlreadyExists)
        {
            const auto errorMessage = std::format(
                "Profile name '{}' already exists",
                _profile.getName()
            );

            LOG_ERROR(errorMessage);

            std::unique_ptr<AddProfileCommandError> errorMessagePtr =
                std::make_unique<AddProfileCommandError>(
                    errorMessage,
                    AddProfileCommandErrorCode::NameAlreadyExists
                );

            return std::unexpected<CommandErrorPtr>(std::move(errorMessagePtr));
        }
        else
        {
            const auto errorMessage = std::format(
                "Unknown error creating profile '{}'",
                _profile.getName()
            );

            LOG_ERROR(errorMessage);

            std::unique_ptr<AddProfileCommandError> errorMessagePtr =
                std::make_unique<AddProfileCommandError>(
                    errorMessage,
                    AddProfileCommandErrorCode::UnknownError
                );

            return std::unexpected<CommandErrorPtr>(std::move(errorMessagePtr));
        }

        return std::expected<void, CommandErrorPtr>{};
    }

    /**
     * @brief Get the Label object
     *
     * @return std::string
     */
    std::string AddProfileCommand::getLabel() const
    {
        return "Create Profile '" + _profile.getName() + "'";
    }

}   // namespace cmd
