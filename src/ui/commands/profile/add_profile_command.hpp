#ifndef __UI__COMMANDS__PROFILE__ADD_PROFILE_COMMAND_HPP__
#define __UI__COMMANDS__PROFILE__ADD_PROFILE_COMMAND_HPP__

#include <string>

#include "drafts/profile_draft.hpp"
#include "ui/commands/command.hpp"
#include "ui/commands/command_error.hpp"

namespace app
{
    class ProfileStore;   // Forward declaration
}   // namespace app

namespace ui
{
    /**
     * @brief A command for adding a profile
     *
     */
    class AddProfileCommand : public ICommand
    {
       private:
        // TODO(97gamjak) [MOLTRACK-204]: make a shared pointer to the profile
        // store
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

        [[nodiscard]] std::expected<void, CommandErrorPtr> undo() override;
        [[nodiscard]] std::expected<void, CommandErrorPtr> redo() override;

        [[nodiscard]] std::string getLabel() const override;
    };
}   // namespace ui

#endif   // __UI__COMMANDS__PROFILE__ADD_PROFILE_COMMAND_HPP__