#include "cache/account_cache.hpp"

#include "finance/account/accounts.hpp"
#include "store/i_account_store.hpp"

namespace cache
{
    /**
     * @brief Construct a new AccountCache:: AccountCache object
     *
     * @param reader Shared pointer to the account store reader
     */
    AccountCache::AccountCache(
        const std::shared_ptr<store::IAccountStoreReader>& reader
    )
        : _reader(reader)
    {
        _addConnection(_reader->subscribeToAdded(
            [this](const auto& account)
            {
                _addAndNotify(
                    account.getId(),
                    std::make_shared<const finance::Account>(account)
                );
            },
            this
        ));

        _addConnection(_reader->subscribeToUpdated(
            [this](const auto& account)
            {
                _update(
                    account.getId(),
                    std::make_shared<const finance::Account>(account)
                );
            },
            this
        ));

        _addConnection(_reader->subscribeToRemoved(
            [this](const auto& accountId) { _remove(accountId); },
            this
        ));

        _addConnection(_reader->subscribeToCommit(
            [this](const auto& idRemap)
            {
                for (const auto& [oldId, newId] : idRemap)
                {
                    _changeId(oldId, newId);
                }
            },
            this
        ));
    }

    /**
     * @brief Load an account from the store using its account ID, this method
     * retrieves the account from the store and returns a shared pointer to the
     * account if it exists, otherwise it returns nullptr.
     *
     * @param accountId The account ID of the account to be loaded from the
     * store.
     * @return std::shared_ptr<const finance::Account> A shared pointer to the
     * loaded account, or nullptr if the account does not exist in the store.
     */
    std::shared_ptr<const finance::Account> AccountCache::_load(
        const AccountId& accountId
    )
    {
        const auto account = _reader->getAccount(accountId);

        if (account)
        {
            return std::make_shared<const finance::Account>(account.value());
        }

        return nullptr;
    }

    /**
     * @brief Load all accounts from the store, this method retrieves all
     * accounts from the store and returns a map of account IDs to shared
     * pointers to the accounts.
     *
     * @return finance::AccountsView::Type A map of account IDs to shared
     * pointers to the loaded accounts.
     */
    finance::AccountsView AccountCache::getAllAccounts()
    {
        return getBulk([this]() { return _loadAll(); }).getItems();
    }

    /**
     * @brief Load all accounts from the store, this method retrieves all
     * accounts from the store and returns a map of account IDs to shared
     * pointers to the accounts.
     *
     * @return finance::AccountsView::Type A map of account IDs to shared
     * pointers to the loaded accounts.
     */
    finance::AccountsView::Type AccountCache::_loadAll()
    {
        const auto accounts = _reader->getAllAccounts();

        finance::AccountsView::Type accountMap;

        for (const auto& account : accounts)
        {
            accountMap.addUnchecked(
                std::make_shared<const finance::Account>(account)
            );
        }

        return accountMap;
    }

}   // namespace cache