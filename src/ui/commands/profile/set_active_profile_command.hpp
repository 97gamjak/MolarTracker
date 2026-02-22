#ifndef __UI__COMMANDS__PROFILE__SET_ACTIVE_PROFILE_COMMAND_HPP__
#define __UI__COMMANDS__PROFILE__SET_ACTIVE_PROFILE_COMMAND_HPP__

#include <expected>
#include <optional>

#include "app/domain/profile.hpp"
#include "ui/commands/command.hpp"

namespace app
{
    class ProfileStore;   // Forward declaration
}

namespace ui
{
    /**
     * @brief A command for setting the active profile
     *
     */
    class SetActiveProfileCommand : public ICommand
    {
       private:
        /// The name of the profile to set as active
        std::string _profileName;

        /// The previous active profile, used for undoing the command
        std::optional<app::Profile> _previousProfile;

        /// Reference to the profile store
        app::ProfileStore& _profileStore;

       public:
        explicit SetActiveProfileCommand(
            std::string        profileName,
            app::ProfileStore& profileStore
        );

        ~SetActiveProfileCommand() override                           = default;
        SetActiveProfileCommand(const SetActiveProfileCommand&)       = delete;
        SetActiveProfileCommand& operator=(const SetActiveProfileCommand&
        )                                                             = delete;
        SetActiveProfileCommand(SetActiveProfileCommand&&)            = delete;
        SetActiveProfileCommand& operator=(SetActiveProfileCommand&&) = delete;

        std::expected<void, CommandErrorPtr> undo() override;
        std::expected<void, CommandErrorPtr> redo() override;

        [[nodiscard]] std::string getLabel() const override;
    };

}   // namespace ui

#endif   // __UI__COMMANDS__PROFILE__SET_ACTIVE_PROFILE_COMMAND_HPP__