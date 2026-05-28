#ifndef __COMMANDS__INCLUDE__COMMANDS__PROFILE__ADD_PROFILE_COMMAND_HPP__
#define __COMMANDS__INCLUDE__COMMANDS__PROFILE__ADD_PROFILE_COMMAND_HPP__

#include <memory>
#include <string>

#include "commands/command.hpp"
#include "commands/command_error.hpp"
#include "domain/profile.hpp"

namespace store
{
    class IProfileStore;   // Forward declaration
}   // namespace store

namespace cmd
{
    /**
     * @brief A command for adding a profile
     *
     */
    class AddProfileCommand : public ICommand
    {
       private:
        /// Reference to the profile store
        std::shared_ptr<store::IProfileStore> _profileStore;
        /// The profile
        domain::Profile _profile;

       public:
        explicit AddProfileCommand(
            const std::shared_ptr<store::IProfileStore>& profileStore,
            domain::Profile                              profile
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
}   // namespace cmd

#endif   // __COMMANDS__INCLUDE__COMMANDS__PROFILE__ADD_PROFILE_COMMAND_HPP__