#ifndef __COMMANDS__INCLUDE__COMMANDS__PROFILE__SET_ACTIVE_PROFILE_COMMAND_HPP__
#define __COMMANDS__INCLUDE__COMMANDS__PROFILE__SET_ACTIVE_PROFILE_COMMAND_HPP__

#include <optional>
#include <string>

#include "app/domain/profile.hpp"
#include "commands/command.hpp"
#include "commands/command_error.hpp"

namespace app
{
    class ProfileStore;   // Forward declaration
}   // namespace app

namespace cmd
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

}   // namespace cmd

#endif   // __COMMANDS__INCLUDE__COMMANDS__PROFILE__SET_ACTIVE_PROFILE_COMMAND_HPP__