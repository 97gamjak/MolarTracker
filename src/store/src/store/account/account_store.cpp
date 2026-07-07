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

        _observable.notify<OnCommit>(getIdRemap());
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

        _observable.notify<OnProfileChanged>();
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
            .filter   = IsAccountActive(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        auto accounts = _getValues(options);

        std::vector<finance::Account> result = {
            accounts.begin(),
            accounts.end()
        };

        for (const auto& account :
             _accountService->getAllAccounts(_activeProfileId))
        {
            if (std::ranges::find_if(
                    result,
                    [&](const finance::Account& account_)
                    { return account_.getId() == account.getId(); }
                ) == result.end())
            {
                result.push_back(account);
            }
        }

        return result;
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
     * @brief Subscribe to commit events in the account store, this allows other
     * parts of the application to be notified when changes are committed in the
     * account store, enabling them to react accordingly, such as updating UI
     * elements or triggering other actions based on the committed changes.
     *
     * @param func The callback function to be called when a commit event
     * occurs, this function should accept a reference to an IdIdMap of
     * AccountId, which represents the mapping of old account IDs to new
     * account IDs after the commit, allowing the subscriber to understand how
     * the account IDs have changed as a result of the commit.
     * @param subscriber A pointer to the subscriber object that will receive
     * the commit event, this allows the account store to manage the lifetime
     * of the subscription and ensure that the callback function is called on
     * the correct subscriber object when a commit event occurs.
     * @return Connection A Connection object representing the subscription,
     * which can be used to manage the subscription, such as disconnecting from
     * the commit events when the subscriber is no longer interested in
     * receiving them, or when the subscriber is being destroyed, ensuring that
     * the subscription is properly cleaned up and does not lead to dangling
     * pointers or memory leaks.
     */
    Connection AccountStore::subscribeToCommit(
        OnCommit::func func,
        void*          subscriber
    )
    {
        return _observable.on<OnCommit>(func, subscriber);
    }

    /**
     * @brief Subscribe to profile changed events in the account store, this
     * allows other parts of the application to be notified when the active
     * profile changes in the account store, enabling them to react accordingly,
     * such as updating UI elements or triggering other actions based on the
     * new active profile.
     *
     * @param func The callback function to be called when a profile changed
     * event occurs, this function should accept no parameters and return void,
     * allowing the subscriber to perform any necessary actions when the active
     * profile changes.
     * @param subscriber A pointer to the subscriber object that will receive
     * the profile changed event, this allows the account store to manage the
     * lifetime of the subscription and ensure that the callback function is
     * called on the correct subscriber object when a profile changed event
     * occurs.
     * @return Connection A Connection object representing the subscription,
     * which can be used to manage the subscription, such as disconnecting from
     * the profile changed events when the subscriber is no longer interested in
     * receiving them, or when the subscriber is being destroyed, ensuring that
     * the subscription is properly cleaned up and does not lead to dangling
     * pointers or memory leaks.
     */
    Connection AccountStore::subscribeToProfileChanged(
        OnProfileChanged::func func,
        void*                  subscriber
    )
    {
        return _observable.on<OnProfileChanged>(func, subscriber);
    }

}   // namespace store