#ifndef __COMMANDS__INCLUDE__COMMANDS__ACCOUNT__CREATE_ACCOUNT_COMMAND_HPP__
#define __COMMANDS__INCLUDE__COMMANDS__ACCOUNT__CREATE_ACCOUNT_COMMAND_HPP__

#include <expected>

#include "commands/command.hpp"
#include "finance/account/account.hpp"

namespace store
{
    class IAccountStore;   // Forward declaration
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
        std::shared_ptr<store::IAccountStore> _accountStore;
        /// The account to create
        finance::Account _account;
        /// The reference account ID, if any
        std::optional<AccountId> _referenceAccount;

       public:
        CreateAccountCommand(
            const std::shared_ptr<store::IAccountStore>& accountStore,
            finance::Account                             account,
            std::optional<AccountId>                     referenceAccount
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
