#include "add_profile_command.hpp"

#include <format>

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

        if (result != app::ProfileStoreResult::Ok)
        {
            // Handle errors (e.g., show a message box) if needed
        }
        else
        {
            LOG_INFO(std::format("Profile removed: '{}'", _profile.name));
        }

        if (_setAsActive)
        {
            const auto _result =
                _profileStore.setActiveProfile(_activeProfileBeforeAdd);

            if (_result != app::ProfileStoreResult::Ok)
            {
                // Handle error if needed
            }
            else
            {
                LOG_INFO(
                    std::format(
                        "Active profile restored to '{}'",
                        _activeProfileBeforeAdd.has_value()
                            ? std::string{_activeProfileBeforeAdd.value()}
                            : "none"
                    )
                );
            }
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

    bool AddProfileCommand::redo()
    {
        const auto result = _profileStore.addProfile(_profile);

        if (result != app::ProfileStoreResult::Ok)
        {
            // Handle errors (e.g., show a message box) if needed
        }
        else
        {
            LOG_INFO(std::format("Profile added: '{}'", _profile.name));
        }

        if (_setAsActive)
        {
            _activeProfileBeforeAdd = _profileStore.getActiveProfileName();
            const auto _result = _profileStore.setActiveProfile(_profile.name);
            if (_result != app::ProfileStoreResult::Ok)
            {
                // Handle error if needed
            }
            else
            {
                LOG_INFO(
                    std::format("Active profile set to '{}'", _profile.name)
                );
            }
        }

        if (_setAsDefault)
        {
            _defaultProfileBeforeAdd = _settings.get_default_profile_name();
            _settings.set_default_profile_name(_profile.name);
            LOG_INFO(std::format("Default profile set to '{}'", _profile.name));
        }

        return true;
    }

    std::string AddProfileCommand::label() const
    {
        return "Add Profile '" + _profile.name + "'";
    }

}   // namespace ui
