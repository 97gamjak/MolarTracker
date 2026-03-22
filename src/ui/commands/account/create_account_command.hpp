#ifndef __UI__COMMANDS__ACCOUNT__CREATE_ACCOUNT_COMMAND_HPP__
#define __UI__COMMANDS__ACCOUNT__CREATE_ACCOUNT_COMMAND_HPP__

#include <expected>

#include "drafts/account_draft.hpp"
#include "ui/commands/command.hpp"

namespace app
{
    class AccountStore;   // Forward declaration
}   // namespace app

namespace ui
{
    /**
     * @brief Command for creating an account
     *
     */
    class CreateAccountCommand : public ICommand
    {
       private:
        /// shared pointer to the account store
        std::shared_ptr<app::AccountStore> _accountStore;
        /// The draft of the account to create
        drafts::AccountDraft _accountDraft;

       public:
        CreateAccountCommand(
            app::AccountStore&   accountStore,
            drafts::AccountDraft accountDraft
        );

        [[nodiscard]] std::expected<void, CommandErrorPtr> redo() override;
        [[nodiscard]] std::expected<void, CommandErrorPtr> undo() override;

        [[nodiscard]] std::string getLabel() const override;
    };

}   // namespace ui

#endif   // __UI__COMMANDS__ACCOUNT__CREATE_ACCOUNT_COMMAND_HPP__