#include "app/store/account_store.hpp"

#include <cassert>
#include <format>

#include "app/services_api/i_account_service.hpp"
#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "drafts/account_draft.hpp"
#include "drafts/account_mapper.hpp"
#include "finance/account.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("App.Store.AccountStore");

using finance::Account;
using finance::HasAccountId;
using finance::HasCurrency;
using finance::IsAccountActive;
using finance::IsAccountType;
using finance::IsExternal;
using std::vector;

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
    [[nodiscard]] AccountStoreResult AccountStore::createAccount(
        const drafts::AccountDraft& account
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
                account.name,
                CurrencyMeta::toString(account.currency)
            )
        );

        const auto newAccount = Account{
            _generateNewId(),
            AccountStatus::Active,
            account.name,
            account.currency,
            account.kind
        };

        _addEntry(newAccount, StoreState::New);

        // special case for cash accounts
        if (account.kind == AccountKind::Cash)
        {
            // check if we already have an external account for this profile id
            // and currency
            const auto existingEntry = _get(
                {.filter   = IsExternal() && HasCurrency(account.currency),
                 .deletion = DeletionPolicy::ExcludeDelete}
            );

            if (!existingEntry.has_value())
            {
                // If no existing entry is found, we can create a new external
                // account
                const auto externalAccount = Account{
                    _generateNewId(),
                    AccountStatus::Active,
                    "External " + CurrencyMeta::toString(account.currency),
                    account.currency,
                    AccountKind::External
                };

                _addEntry(externalAccount, StoreState::New);
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

        _clearChangedIds();

        for (auto& entry : _getEntries())
        {
            switch (entry.state)
            {
                case StoreState::New:
                {
                    auto&      account = entry.value;
                    const auto oldId   = account.getId();
                    auto       id      = _accountService->createAccount(
                        account,
                        _activeProfileId
                    );

                    account.setId(id);
                    _appendChangedIds(oldId, id);
                    entry.state = StoreState::Clean;

                    LOG_INFO(
                        std::format(
                            "Account '{}' added to database",
                            account.getName()
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

        assert(!isDirty());
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
        _clearEntries();

        if (_activeProfileId.isValid())
        {
            const auto accounts =
                _accountService->getAllAccounts(_activeProfileId);

            for (const auto& account : accounts)
                _addEntry(account, StoreState::Clean);
        }
    }

    /**
     * @brief Retrieves a vector of pointers to all accounts currently
     * in the store, this allows callers to access the accounts managed
     * by the store, and provides a way to retrieve the account data for
     * display or further processing.
     *
     * @return std::vector<const Account*> A vector of pointers
     * to all accounts currently in the store, each pointer points to an
     * account object that is managed by the store, and the caller can
     * use these pointers to access the account data and perform
     * operations on the accounts as needed.
     */
    std::vector<drafts::AccountDraft> AccountStore::getAllAccounts() const
    {
        const auto options = Options{
            .filter   = !IsExternal() && IsAccountActive(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        auto accounts = _getValues(options) |
                        std::views::transform(drafts::AccountMapper::toDraft);

        return {accounts.begin(), accounts.end()};
    }

    std::vector<drafts::AccountDraft> AccountStore::getCashAccounts() const
    {
        const auto options = Options{
            .filter   = IsAccountType(AccountKind::Cash) && IsAccountActive(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        auto accounts = _getValues(options) |
                        std::views::transform(drafts::AccountMapper::toDraft);

        return {accounts.begin(), accounts.end()};
    }

    std::unordered_map<AccountId, std::string, AccountId::Hash> AccountStore::
        getAccountIdToNameMap() const
    {
        const auto options = Options{
            .filter   = IsAccountActive(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        std::unordered_map<AccountId, std::string, AccountId::Hash>
            accountIdToName;

        for (const auto& account : _getValues(options))
        {
            accountIdToName.emplace(account.getId(), account.getName());
        }

        return accountIdToName;
    }

    AccountId AccountStore::getExternalAccount(Currency currency) const
    {
        const auto options = Options{
            .filter   = IsExternal() && HasCurrency(currency),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        const auto account = _get(options);

        if (account.has_value())
            return account->getId();

        throw AccountStoreException(
            "No external account found for currency " +
            CurrencyMeta::toString(currency)
        );
    }

    /**
     * @brief Get an account by its ID
     *
     * @param id The ID of the account to retrieve
     * @return std::optional<drafts::AccountDraft> The account if
     * found, or an empty optional if not found
     */
    std::optional<drafts::AccountDraft> AccountStore::getAccount(
        AccountId id
    ) const
    {
        const auto options = Options{
            .filter   = HasAccountId(id) && IsAccountActive(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        const auto account = _get(options);

        if (account.has_value())
            return drafts::AccountMapper::toDraft(account.value());

        return std::nullopt;
    }

}   // namespace app