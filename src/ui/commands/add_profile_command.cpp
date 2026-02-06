#include "add_profile_command.hpp"

#include <expected>
#include <format>

#include "add_profile_command_error.hpp"
#include "app/store/profile_store.hpp"
#include "drafts/profile_draft.hpp"
#include "settings/settings.hpp"

#define __LOG_CATEGORY__ LogCategory::ui_command_AddProfileCommand
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
        settings::Settings&  settings,
        drafts::ProfileDraft profile,
        bool                 setAsActive,
        bool                 setAsDefault
    )
        : _profileStore{profileStore},
          _settings{settings},
          _profile{profile},
          _setAsActive{setAsActive},
          _setAsDefault{setAsDefault}
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

        if (_setAsActive)
        {
            // here no check is done if old profile exists... we will let this
            // fail with an exception as this should really really never happen
            _profileStore.setActiveProfile(_activeProfileBeforeAdd);

            const auto name = _activeProfileBeforeAdd.has_value()
                                  ? std::string{_activeProfileBeforeAdd.value()}
                                  : "none";

            const auto msg =
                std::format("Active profile restored to '{}'", name);

            LOG_INFO(msg);
        }

        if (_setAsDefault)
        {
            _settings.setDefaultProfileName(_defaultProfileBeforeAdd);

            const auto name =
                _defaultProfileBeforeAdd.has_value()
                    ? std::string{_defaultProfileBeforeAdd.value()}
                    : "none";

            const auto msg =
                std::format("Default profile restored to '{}'", name);

            LOG_INFO(msg);
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
            LOG_INFO(std::format("Profile added: '{}'", _profile.name));
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
            const auto errorMessage =
                std::format("Unknown error adding profile '{}'", _profile.name);

            LOG_ERROR(errorMessage);

            std::unique_ptr<AddProfileCommandError> errorMessagePtr =
                std::make_unique<AddProfileCommandError>(
                    errorMessage,
                    AddProfileCommandErrorCode::UnknownError
                );

            return std::unexpected<CommandErrorPtr>(std::move(errorMessagePtr));
        }

        if (_setAsActive)
        {
            _activeProfileBeforeAdd = _profileStore.getActiveProfileName();
            _profileStore.setActiveProfile(_profile.name);

            LOG_INFO(std::format("Active profile set to '{}'", _profile.name));
        }

        if (_setAsDefault)
        {
            _defaultProfileBeforeAdd = _settings.getDefaultProfileName();
            _settings.setDefaultProfileName(_profile.name);
            LOG_INFO(std::format("Default profile set to '{}'", _profile.name));
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
        return "Add Profile '" + _profile.name + "'";
    }

}   // namespace ui
