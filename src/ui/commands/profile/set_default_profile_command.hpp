#ifndef __UI__COMMANDS__SET_DEFAULT_PROFILE_COMMAND_HPP__
#define __UI__COMMANDS__SET_DEFAULT_PROFILE_COMMAND_HPP__

#include <expected>
#include <optional>
#include <string>

#include "ui/commands/command.hpp"

namespace settings
{
    class GeneralSettings;   // Forward declaration
}

namespace ui
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

}   // namespace ui

#endif   // __UI__COMMANDS__SET_DEFAULT_PROFILE_COMMAND_HPP__