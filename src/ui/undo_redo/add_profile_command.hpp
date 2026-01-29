#ifndef __UI__UNDO_REDO__ADD_PROFILE_COMMAND_HPP__
#define __UI__UNDO_REDO__ADD_PROFILE_COMMAND_HPP__

#include <optional>
#include <string_view>

#include "command.hpp"
#include "drafts/profile_draft.hpp"

namespace app
{
    class ProfileStore;   // Forward declaration
}

namespace settings
{
    class Settings;   // Forward declaration
}

namespace ui
{
    class AddProfileCommand : public ICommand
    {
       private:
        app::ProfileStore&   _profileStore;
        settings::Settings&  _settings;
        drafts::ProfileDraft _profile;

        bool _setAsActive  = false;
        bool _setAsDefault = false;

        std::optional<std::string_view> _activeProfileBeforeAdd  = std::nullopt;
        std::optional<std::string>      _defaultProfileBeforeAdd = std::nullopt;

       public:
        AddProfileCommand(
            app::ProfileStore&   profileStore,
            settings::Settings&  settings,
            drafts::ProfileDraft profile,
            bool                 setAsActive,
            bool                 setAsDefault
        );
        ~AddProfileCommand() override = default;

        void undo() override;
        bool redo() override;

        std::string label() const override;
    };
}   // namespace ui

#endif   // __UI__UNDO_REDO__ADD_PROFILE_COMMAND_HPP__