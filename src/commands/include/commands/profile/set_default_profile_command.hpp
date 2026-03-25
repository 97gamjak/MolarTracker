#ifndef __UI__COMMANDS__PROFILE__SET_DEFAULT_PROFILE_COMMAND_HPP__
#define __UI__COMMANDS__PROFILE__SET_DEFAULT_PROFILE_COMMAND_HPP__

#include <optional>
#include <string>

#include "commands/command.hpp"
#include "commands/command_error.hpp"

namespace settings
{
    class GeneralSettings;   // Forward declaration
}   // namespace settings

namespace cmd
{
    /**
     * @brief A command for setting the default profile
     *
     */
    class SetDefaultProfileCommand : public ICommand
    {
       private:
        /// The name of the profile to set as default
        std::optional<std::string> _profileName;

        /// The previous default profile name, used for undoing the command
        std::optional<std::string> _previousProfileName;

        /// Reference to the general settings
        settings::GeneralSettings& _settings;

       public:
        explicit SetDefaultProfileCommand(
            std::string                profileName,
            settings::GeneralSettings& settings
        );

        ~SetDefaultProfileCommand() override                      = default;
        SetDefaultProfileCommand(const SetDefaultProfileCommand&) = delete;
        SetDefaultProfileCommand& operator=(const SetDefaultProfileCommand&
        )                                                         = delete;
        SetDefaultProfileCommand(SetDefaultProfileCommand&&)      = delete;
        SetDefaultProfileCommand& operator=(SetDefaultProfileCommand&&
        )                                                         = delete;

        std::expected<void, CommandErrorPtr> undo() override;
        std::expected<void, CommandErrorPtr> redo() override;

        [[nodiscard]] std::string getLabel() const override;
    };

}   // namespace cmd

#endif   // __UI__COMMANDS__PROFILE__SET_DEFAULT_PROFILE_COMMAND_HPP__