#include "account_store.hpp"

#include "app/services_api/i_account_service.hpp"
#include "drafts/account_draft.hpp"
#include "finance/cash_account.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("App.Store.AccountStore");

namespace app
{

    /**
     * @brief Construct a new Account Store object
     *
     * @param accountService A shared pointer to the account service that the
     * store will use to perform operations related to accounts, this allows the
     * store to interact with the underlying business logic and data management
     * for accounts, and ensures that the store can access the necessary methods
     * and data to manage accounts effectively.
     */
    AccountStore::AccountStore(
        const std::shared_ptr<IAccountService>& accountService
    )
        : _accountService(accountService)
    {
        const auto accounts = _accountService->getAllAccounts(_activeProfileId);

        for (const auto& account : accounts)
            _addEntry(account, StoreState::Clean);
    }

    /**
     * @brief Create a new account based on the given account draft
     *
     * @param accountDraft A draft containing the necessary information to
     * create a new account, this allows the store to take the user input and
     * convert it into a format that can be used to create a new account in the
     * underlying service, and ensures that the store can validate and process
     * the input before attempting to create the account.
     * @return AccountStoreResult The result of the create operation, this
     * allows the caller to understand whether the account was created
     * successfully or if there was an error, and provides information about
     * what went wrong if the creation failed.
     */
    [[nodiscard]] AccountStoreResult AccountStore::createAccount(
        const drafts::AccountDraft& accountDraft
    )
    {
        if (accountDraft.kind == AccountKind::Cash)
            return _addCashAccount(accountDraft);

        const auto msg = "Unsupported account kind: " +
                         AccountKindMeta::toString(accountDraft.kind);

        LOG_ERROR(msg);
        throw AccountStoreException(msg);
    }

    /**
     * @brief Commit any pending changes in the store to the underlying service
     *
     */
    void AccountStore::commit() {}

    /**
     * @brief Add a new cash account based on the given account draft
     *
     * @param accountDraft A draft containing the necessary information to
     * create a new cash account, this allows the store to take the user input
     * and convert it into a format that can be used to create a new cash
     * account in the underlying service, and ensures that the store can
     * validate and process the input before attempting to create the account.
     * @return AccountStoreResult The result of the add operation, this allows
     * the caller to understand whether the account was added successfully or if
     * there was an error, and provides information about what went wrong if the
     * addition failed.
     */
    [[nodiscard]] AccountStoreResult AccountStore::_addCashAccount(
        const drafts::AccountDraft& accountDraft
    )
    {
        const auto account = finance::CashAccount{
            _generateNewId(),
            AccountStatus::Active,
            accountDraft.name,
            accountDraft.currency
        };

        _addEntry(account, StoreState::New);

        return AccountStoreResult::Ok;
    }

}   // namespace app