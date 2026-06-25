#include "store/account/account_store.hpp"

#include <cassert>
#include <format>
#include <ranges>

#include "config/id_types.hpp"
#include "finance/account/account.hpp"
#include "logging/log_macros.hpp"
#include "service/i_account_service.hpp"
#include "utils/finance.hpp"

REGISTER_LOG_CATEGORY("Store.AccountStore");

using finance::HasAccountId;
using finance::HasCurrency;
using finance::HasName;
using finance::IsAccountActive;
using finance::IsAccountType;
using finance::IsExternal;

namespace store
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
        const std::shared_ptr<service::IAccountService>& accountService
    )
        : _accountService(accountService)
    {
        // we use the unchecked versions as ids are unique due to store
        // handling!
        _connections.add(subscribeToEntryAdded(
            [this](const std::vector<finance::Account>& accounts)
            { _session.addUnchecked(accounts); },
            this
        ));

        _connections.add(subscribeToEntryRemoved(
            [this](const std::vector<AccountId>& accountIds)
            { _session.removeUnchecked(accountIds); },
            this
        ));

        _refresh();
    }

    /**
     * @brief Create a new account based on the given account draft
     *
     * @param account A draft containing the necessary information to
     * create a new account, this allows the store to take the user input and
     * convert it into a format that can be used to create a new account in the
     * underlying service, and ensures that the store can validate and process
     * the input before attempting to create the account.
     * @return AccountStoreResult The result of the create operation, this
     * allows the caller to understand whether the account was created
     * successfully or if there was an error, and provides information about
     * what went wrong if the creation failed.
     */
    AccountStoreResult AccountStore::createAccount(
        const finance::Account& account
    )
    {
        if (_activeProfileId == ProfileId::invalid())
        {
            LOG_ERROR("Cannot create account without an active profile");
            return AccountStoreResult::Error;
        }

        LOG_DEBUG(
            std::format(
                "Creating account with name '{}' and currency '{}'",
                account.getName(),
                CurrencyMeta::toString(account.getCurrency())
            )
        );

        const auto options = Options{
            .filter =
                IsAccountType(account.getKind()) && HasName(account.getName()),
            .deletion = DeletionPolicy::ExcludeDelete
        };
        const auto existingAccount = _get(options);

        if (existingAccount.has_value())
        {
            LOG_ERROR(
                std::format(
                    "Account with name '{}' and type '{}' already exists",
                    account.getName(),
                    AccountKindMeta::toString(account.getKind())
                )
            );
            return AccountStoreResult::AccountNameConflict;
        }

        const auto newAccount = finance::Account{
            AccountId::invalid(),
            AccountStatus::Active,
            account.getName(),
            account.getCurrency(),
            account.getKind()
        };

        _addEntry(newAccount);

        // special case for cash accounts
        if (account.getKind() == AccountKind::Cash)
        {
            // check if we already have an external account for this profile id
            // and currency
            const auto existingEntry = _get(
                {.filter   = IsExternal() && HasCurrency(account.getCurrency()),
                 .deletion = DeletionPolicy::ExcludeDelete}
            );

            if (!existingEntry.has_value())
            {
                // If no existing entry is found, we can create a new external
                // account
                const auto externalAccount = finance::Account{
                    AccountId::invalid(),
                    AccountStatus::Active,
                    "External " + CurrencyMeta::toString(account.getCurrency()),
                    account.getCurrency(),
                    AccountKind::External
                };

                _addEntry(externalAccount);
            }
        }

        return AccountStoreResult::Ok;
    }

    /**
     * @brief Commit any pending changes in the store to the underlying service
     *
     */
    void AccountStore::commit()
    {
        LOG_ENTRY;

        for (const auto& entry : _getEntries())
        {
            switch (entry.state)
            {
                case StoreState::New:
                {
                    auto       newEntry = entry;
                    const auto oldId    = newEntry.value.getId();
                    auto       id       = _accountService->createAccount(
                        newEntry.value,
                        _activeProfileId
                    );
                    newEntry.value.setId(id);

                    const auto result = _commitEntry(oldId, newEntry);

                    if (result != StoreResult::Ok)
                    {
                        throw AccountStoreException(
                            std::format(
                                "Failed to add account '{}' to database",
                                newEntry.value.getName()
                            )
                        );
                    }

                    LOG_INFO(
                        std::format(
                            "Account '{}' added to database",
                            newEntry.value.getName()
                        )
                    );
                    break;
                }
                case StoreState::Clean:
                {
                    break;
                }
                case StoreState::Modified:
                case StoreState::Deleted:
                {
                    throw AccountStoreException(
                        "Store state " +
                        std::to_string(static_cast<int>(entry.state)) +
                        " not supported yet"
                    );
                }
            }
        }

        // here now we set our ids because they are now clean!
        // we use the unchecked version as ids are unique due to store handling!
        _session.setUnchecked(_getValues());
    }

    /**
     * @brief Update the active profile for the store, this will
     * determine which accounts are loaded and managed in the store, and
     * should be called whenever the active profile changes to ensure
     * that the store is managing the correct set of accounts for the
     * current profile.
     *
     * @param profileIdOpt An optional containing the ID of the new
     * active profile, if std::nullopt is passed, it indicates that
     * there is no active profile, and the store should clear its data
     * and not manage any accounts.
     */
    void AccountStore::updateActiveProfile(
        const std::optional<ProfileId>& profileIdOpt
    )
    {
        if (!profileIdOpt.has_value())
        {
            LOG_WARNING(
                "No active profile set, ignoring updateActiveProfile "
                "call"
            );
            _activeProfileId = ProfileId::invalid();
            _clearEntries();
            return;
        }

        const auto profileId = profileIdOpt.value();

        if (profileId == _activeProfileId)
            return;

        LOG_INFO(
            std::format(
                "Updating active profile in AccountStore to '{}'",
                profileId.value()
            )
        );

        _activeProfileId = profileId;

        // we can safely refresh the store here because it has only an affect if
        // the store is not dirty, and the store should not be dirty when the
        // profile is updated. If the store is dirty, it means that there are
        // unsaved changes and we probably update from an invalid profile id
        _refresh();
    }

    /**
     * @brief Refresh the store's data by clearing existing entries and
     * loading accounts from the underlying service for the active
     * profile, this should be called whenever the active profile
     * changes or when the store needs to ensure that it has the most
     * up-to-date data from the service, and will repopulate the store
     * with accounts that are relevant to the current active profile.
     *
     */
    void AccountStore::_refresh()
    {
        // We really only want to refresh if we are not dirty, otherwise we
        // might lose unsaved changes
        if (isDirty())
            return;

        _clearEntries();

        if (_activeProfileId.isValid())
        {
            LOG_DEBUG(
                std::format(
                    "Refreshing account store for active profile: {}",
                    _activeProfileId.value()
                )
            );

            const auto accounts =
                _accountService->getAllAccounts(_activeProfileId);

            LOG_DEBUG(std::format("Retrieved accounts: {}", accounts.size()));

            _addCleanEntries(accounts);
            _session.setUnchecked(_getValues());
        }
    }

    /**
     * @brief Retrieves a vector of account drafts representing all accounts
     * currently in the store, this allows callers to get a list of all accounts
     * managed by the store in a format that is suitable for display or further
     * processing, and provides a way to access the account data in the store as
     * a collection of drafts that can be used for various purposes such as
     * populating UI elements or performing operations on the accounts.
     *
     * @return std::vector<finance::Account>
     */
    std::vector<finance::Account> AccountStore::getAllAccounts() const
    {
        const auto options = Options{
            .filter   = !IsExternal() && IsAccountActive(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        auto accounts = _getValues(options);

        return {accounts.begin(), accounts.end()};
    }

    /**
     * @brief get all cash accounts in the store, this allows callers to
     * retrieve only the cash accounts managed by the store, which can be useful
     * for display purposes or for operations that specifically involve cash
     * accounts, and provides a way to filter the accounts based on their type
     * and status.
     *
     * @return std::vector<finance::Account> A vector of account drafts
     * representing the cash accounts currently in the store, each draft
     * contains the necessary information about a cash account that can be used
     * for display or further processing, and the caller can use this vector to
     * access the cash account data as needed.
     */
    std::vector<finance::Account> AccountStore::getCashAccounts() const
    {
        const auto options = Options{
            .filter   = IsAccountType(AccountKind::Cash) && IsAccountActive(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        auto accounts = _getValues(options);

        return {accounts.begin(), accounts.end()};
    }

    /**
     * @brief Get all security accounts in the store
     *
     * This allows callers to retrieve only the security accounts managed by the
     * store, which can be useful for display purposes or for operations that
     * specifically involve security accounts, and provides a way to filter the
     * accounts based on their type and status.
     *
     * @return std::vector<finance::Account> A vector of account drafts
     * representing the security accounts currently in the store, each draft
     * contains the necessary information about a security account that can be
     * used for display or further processing, and the caller can use this
     * vector to access the security account data as needed.
     */
    std::vector<finance::Account> AccountStore::getSecurityAccounts() const
    {
        const auto options = Options{
            .filter = IsAccountType(AccountKind::Security) && IsAccountActive(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        auto accounts = _getValues(options);

        return {accounts.begin(), accounts.end()};
    }

    /**
     * @brief Get a mapping of account IDs to account names for all active
     * accounts in the store, this allows callers to easily look up the name of
     * an account based on its ID, which can be useful for display purposes or
     * when performing operations that require referencing accounts by their ID,
     * and provides a convenient way to access the account names without having
     * to retrieve the full account data for each account.
     *
     * @return unorderedIdMap<AccountId, std::string> A
     * map where the keys are account IDs and the values are the corresponding
     * account names for all active accounts in the store, this allows for
     * efficient lookups of account names based on their IDs and can be used in
     * various parts of the application where such mappings are needed.
     */
    unorderedIdMap<AccountId, std::string> AccountStore::getAccountIdToNameMap(
    ) const
    {
        const auto options = Options{
            .filter   = IsAccountActive(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        unorderedIdMap<AccountId, std::string> accountIdToName;

        for (const auto& account : _getValues(options))
        {
            accountIdToName.emplace(account.getId(), account.getName());
        }

        return accountIdToName;
    }

    /**
     * @brief Get the ID of the external account for a given currency, this is
     * used to find the corresponding external account for a cash account based
     * on its currency, and allows callers to retrieve the ID of the external
     * account that is associated with a specific currency, which can be useful
     * for operations that involve cash accounts and their corresponding
     * external accounts.
     *
     * @param currency The currency for which to find the external account, this
     * specifies the currency of the cash account for which we want to find the
     * corresponding external account, and is used as a filter criterion when
     * searching for the external account in the store.
     * @return AccountId The ID of the external account associated with the
     * given currency, if found, this is the identifier of the external account
     * that corresponds to the specified currency, and can be used for various
     * operations that require referencing the external account.
     */
    std::optional<AccountId> AccountStore::getExternalAccount(
        Currency currency
    ) const
    {
        const auto options = Options{
            .filter   = IsExternal() && HasCurrency(currency),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        const auto account = _get(options);

        if (account.has_value())
            return account->getId();

        return std::nullopt;
    }

    /**
     * @brief Get the IDs of all external accounts
     *
     * @return IdSet<AccountId>
     */
    IdSet<AccountId> AccountStore::getExternalAccountIds() const
    {
        const auto options = Options{
            .filter   = IsExternal(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        IdSet<AccountId> externalAccountIds;

        for (const auto& account : _getValues(options))
            externalAccountIds.insert(account.getId());

        return externalAccountIds;
    }

    /**
     * @brief Get an account by its ID
     *
     * @param id The ID of the account to retrieve
     * @return std::optional<finance::Account> The account if
     * found, or an empty optional if not found
     */
    std::optional<finance::Account> AccountStore::getAccount(AccountId id) const
    {
        const auto options = Options{
            .filter   = HasAccountId(id) && IsAccountActive(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        return _get(options);
    }

    /**
     * @brief Get the account session
     *
     * @return const finance::Accounts& The account session
     */
    const finance::Accounts& AccountStore::getAccountSession() const
    {
        return _session;
    }

    /**
     * @brief Get the ID remapping for accounts
     *
     * @return const IdIdMap<AccountId>& The ID remapping
     */
    const IdIdMap<AccountId>& AccountStore::getIdRemap() const
    {
        return _getIdRemap();
    }

}   // namespace store