#include "commands/account/create_account_command.hpp"

#include <expected>
#include <format>

#include "finance/account/account.hpp"
#include "logging/log_macros.hpp"
#include "store/i_account_store.hpp"

REGISTER_LOG_CATEGORY("UI.Commands.CreateAccountCommand");

namespace cmd
{
    /**
     * @brief Construct a new Create Account Command:: Create Account Command
     * object
     *
     * @param accountStore
     * @param account
     * @param referenceAccount
     */
    CreateAccountCommand::CreateAccountCommand(
        const std::shared_ptr<store::IAccountStore>& accountStore,
        finance::Account                             account,
        std::optional<AccountId>                     referenceAccount
    )
        : _accountStore(accountStore),
          _account(std::move(account)),
          _referenceAccount(referenceAccount)
    {
    }

    /**
     * @brief Get the Label object
     *
     * @return std::string
     */
    std::string CreateAccountCommand::getLabel() const
    {
        return "Create Account";
    }

    /**
     * @brief Undo the create account command
     *
     * @return std::expected<void, CommandErrorPtr> Result of the operation
     */
    std::expected<void, CommandErrorPtr> CreateAccountCommand::undo()
    {
        // TODO(97gamjak): implement undo for create account
        // command
        return std::unexpected<CommandErrorPtr>(
            std::make_unique<CommandError>(CommandErrorType::UndoNotImplemented)
        );
    }

    /**
     * @brief Redo the create account command
     *
     * @return std::expected<void, CommandErrorPtr> Result of the operation
     */
    std::expected<void, CommandErrorPtr> CreateAccountCommand::redo()
    {
        const auto result = _accountStore->createAccount(_account);
        if (result != store::AccountStoreResult::Ok)
        {
            const auto errorMessage = std::format(
                "Failed to create account '{}'",
                _account.getName()
            );

            LOG_ERROR(errorMessage);

            // TODO(97gamjak): create specific error types for account creation
            // errors
            std::unique_ptr<CommandError> error =
                std::make_unique<CommandError>(CommandErrorType::InvalidCommand
                );

            return std::unexpected<CommandErrorPtr>(std::move(error));
        }

        if (_referenceAccount.has_value())
        {
            const auto linkResult = _accountStore->linkAccounts(
                _account.getId(),
                _referenceAccount.value()
            );

            if (!linkResult)
            {
                const auto errorMessage = std::format(
                    "Failed to link account '{}' to reference account '{}'",
                    _account.getName(),
                    _referenceAccount.value().value()
                );

                LOG_ERROR(errorMessage);

                // TODO(97gamjak): create specific error types for account
                // linking errors
                std::unique_ptr<CommandError> error =
                    std::make_unique<CommandError>(
                        CommandErrorType::InvalidCommand
                    );

                return std::unexpected<CommandErrorPtr>(std::move(error));
            }
        }

        return {};
    }

}   // namespace cmd
