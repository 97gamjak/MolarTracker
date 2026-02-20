#ifndef __UI__COMMANDS__ADD_PROFILE_COMMAND_HPP__
#define __UI__COMMANDS__ADD_PROFILE_COMMAND_HPP__

#include <expected>
#include <optional>

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
    /**
     * @brief A command for adding a profile
     *
     */
    class AddProfileCommand : public ICommand
    {
       private:
        /// Reference to the profile store
        app::ProfileStore& _profileStore;
        /// Reference to the settings
        settings::Settings& _settings;
        /// The profile draft
        drafts::ProfileDraft _profile;

        /// Whether to set the new profile as active
        bool _setAsActive = false;
        /// Whether to set the new profile as default
        bool _setAsDefault = false;

        /// The active profile before adding the new profile
        std::optional<std::string> _activeProfileBeforeAdd = std::nullopt;
        /// The default profile before adding the new profile
        std::optional<std::string> _defaultProfileBeforeAdd = std::nullopt;

       public:
        AddProfileCommand(
            app::ProfileStore&   profileStore,
            settings::Settings&  settings,
            drafts::ProfileDraft profile,
            bool                 setAsActive,
            bool                 setAsDefault
        );
        ~AddProfileCommand() override = default;

        std::expected<void, CommandErrorPtr> undo() override;
        std::expected<void, CommandErrorPtr> redo() override;

        std::string getLabel() const override;
    };
}   // namespace ui

#endif   // __UI__COMMANDS__ADD_PROFILE_COMMAND_HPP__