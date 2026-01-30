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

    void AddProfileCommand::undo()
    {
        const auto result = _profileStore.removeProfile(_profile);

        if (result == app::ProfileStoreResult::ProfileNotFound)
        {
            // TODO: think about handling errors also on undo
            LOG_ERROR(
                std::format(
                    "Failed to undo add profile: '{}' not found",
                    _profile.name
                )
            );
        }
        else
        {
            LOG_INFO(std::format("Profile removed: '{}'", _profile.name));
        }

        if (_setAsActive)
        {
            _profileStore.setActiveProfile(_activeProfileBeforeAdd);

            LOG_INFO(
                std::format(
                    "Active profile restored to '{}'",
                    _activeProfileBeforeAdd.has_value()
                        ? std::string{_activeProfileBeforeAdd.value()}
                        : "none"
                )
            );
        }

        if (_setAsDefault)
        {
            _settings.set_default_profile_name(_defaultProfileBeforeAdd);
            LOG_INFO(
                std::format(
                    "Default profile restored to '{}'",
                    _defaultProfileBeforeAdd.has_value()
                        ? _defaultProfileBeforeAdd.value()
                        : "none"
                )
            );
        }
    }

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
            _defaultProfileBeforeAdd = _settings.get_default_profile_name();
            _settings.set_default_profile_name(_profile.name);
            LOG_INFO(std::format("Default profile set to '{}'", _profile.name));
        }

        return std::expected<void, CommandErrorPtr>{};
    }

    std::string AddProfileCommand::label() const
    {
        return "Add Profile '" + _profile.name + "'";
    }

}   // namespace ui
