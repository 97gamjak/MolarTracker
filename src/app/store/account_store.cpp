#include "account_store.hpp"

#include "app/services_api/i_account_service.hpp"

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
     * @brief Commit any pending changes in the store to the underlying service
     *
     */
    void AccountStore::commit() {}

}   // namespace app