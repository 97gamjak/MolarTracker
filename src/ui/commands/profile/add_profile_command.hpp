#ifndef __UI__COMMANDS__ADD_PROFILE_COMMAND_HPP__
#define __UI__COMMANDS__ADD_PROFILE_COMMAND_HPP__

#include <expected>

#include "drafts/profile_draft.hpp"
#include "ui/commands/command.hpp"

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
        /// The profile draft
        drafts::ProfileDraft _profile;

       public:
        explicit AddProfileCommand(
            app::ProfileStore&   profileStore,
            drafts::ProfileDraft profile
        );
        ~AddProfileCommand() override                          = default;
        AddProfileCommand(const AddProfileCommand&)            = delete;
        AddProfileCommand& operator=(const AddProfileCommand&) = delete;
        AddProfileCommand(AddProfileCommand&&)                 = delete;
        AddProfileCommand& operator=(AddProfileCommand&&)      = delete;

        std::expected<void, CommandErrorPtr> undo() override;
        std::expected<void, CommandErrorPtr> redo() override;

        std::string getLabel() const override;
    };
}   // namespace ui

#endif   // __UI__COMMANDS__ADD_PROFILE_COMMAND_HPP__