#ifndef __COMMANDS__INCLUDE__COMMANDS__ACCOUNT__CREATE_ACCOUNT_COMMAND_HPP__
#define __COMMANDS__INCLUDE__COMMANDS__ACCOUNT__CREATE_ACCOUNT_COMMAND_HPP__

#include <expected>

#include "commands/command.hpp"
#include "drafts/account_draft.hpp"

namespace store
{
    class AccountStore;   // Forward declaration
}   // namespace store

namespace cmd
{
    /**
     * @brief Command for creating an account
     *
     */
    class CreateAccountCommand : public ICommand
    {
       private:
        /// Reference to the account store
        store::AccountStore& _accountStore;
        /// The draft of the account to create
        drafts::AccountDraft _accountDraft;

       public:
        CreateAccountCommand(
            store::AccountStore& accountStore,
            drafts::AccountDraft accountDraft
        );

        ~CreateAccountCommand() override                             = default;
        CreateAccountCommand(const CreateAccountCommand&)            = delete;
        CreateAccountCommand& operator=(const CreateAccountCommand&) = delete;
        CreateAccountCommand(CreateAccountCommand&&)                 = delete;
        CreateAccountCommand& operator=(CreateAccountCommand&&)      = delete;

        [[nodiscard]] std::expected<void, CommandErrorPtr> redo() override;
        [[nodiscard]] std::expected<void, CommandErrorPtr> undo() override;

        [[nodiscard]] std::string getLabel() const override;
    };

}   // namespace cmd

#endif   // __COMMANDS__INCLUDE__COMMANDS__ACCOUNT__CREATE_ACCOUNT_COMMAND_HPP__