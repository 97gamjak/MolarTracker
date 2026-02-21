#include "add_profile_command.hpp"

#include <expected>
#include <format>

#include "add_profile_command_error.hpp"
#include "app/store/profile_store.hpp"
#include "drafts/profile_draft.hpp"

#define __LOG_CATEGORY__ LogCategory::ui_AddProfileCommand
#include "logging/log_macros.hpp"

namespace ui
{
    /**
     * @brief Construct a new Add Profile Command:: Add Profile Command object
     *
     * @param profileStore
     * @param settings
     * @param profile
     * @param setAsActive
     * @param setAsDefault
     */
    AddProfileCommand::AddProfileCommand(
        app::ProfileStore&   profileStore,
        drafts::ProfileDraft profile
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
        const auto result = _profileStore.removeProfile(_profile);

        if (result == app::ProfileStoreResult::ProfileNotFound)
        {
            const auto errorMessage = std::format(
                "Failed to undo add profile: '{}' not found",
                _profile.name
            );

            LOG_ERROR(errorMessage);

            std::unique_ptr<AddProfileCommandError> error =
                std::make_unique<AddProfileCommandError>(
                    errorMessage,
                    AddProfileCommandErrorCode::ProfileNotFound
                );

            return std::unexpected<CommandErrorPtr>(std::move(error));
        }
        else if (result == app::ProfileStoreResult::Ok)
        {
            LOG_INFO(std::format("Profile removed: '{}'", _profile.name));
        }
        else
        {
            const auto errorMessage = std::format(
                "Unknown error removing profile '{}'",
                _profile.name
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
        const auto result = _profileStore.addProfile(_profile);

        if (result == app::ProfileStoreResult::Ok)
        {
            LOG_INFO(std::format("Profile created: '{}'", _profile.name));
        }
        else if (result == app::ProfileStoreResult::NameAlreadyExists)
        {
            const auto errorMessage =
                std::format("Profile name '{}' already exists", _profile.name);

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
                _profile.name
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
        return "Create Profile '" + _profile.name + "'";
    }

}   // namespace ui
