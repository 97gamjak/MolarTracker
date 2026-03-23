#include "account_store.hpp"

#include "app/services_api/i_account_service.hpp"
#include "config/finance_enums.hpp"
#include "config/id_types.hpp"
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
        _refresh();
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
        if (_activeProfileId == ProfileId::invalid())
        {
            LOG_ERROR("Cannot create account without an active profile");
            return AccountStoreResult::Error;
        }

        LOG_DEBUG(
            std::format(
                "Creating account with name '{}' and currency '{}'",
                accountDraft.name,
                CurrencyMeta::toString(accountDraft.currency)
            )
        );

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
    void AccountStore::commit()
    {
        LOG_ENTRY;

        _clearChangedIds();

        for (auto& entry : _getEntries())
        {
            if (entry.state == StoreState::New)
            {
                if (!std::holds_alternative<finance::CashAccount>(entry.value))
                {
                    auto msg =
                        "Unsupported account type in commit: " +
                        std::string(
                            entry.value.index() == 0 ? "CashAccount" : "Unknown"
                        );
                    LOG_ERROR(msg);
                    throw AccountStoreException(msg);
                }

                auto& cashAccount = std::get<finance::CashAccount>(entry.value);
                const auto oldId  = cashAccount.getId();
                auto       id     = _accountService->createCashAccount(
                    cashAccount,
                    _activeProfileId
                );
                cashAccount.setId(id);
                _appendChangedIds(oldId, id);
                entry.state = StoreState::Clean;

                LOG_INFO(
                    std::format(
                        "Account '{}' added to database",
                        std::visit(
                            [](const auto& acc) { return acc.getName(); },
                            entry.value
                        )
                    )
                );
            }
            else if (entry.state == StoreState::Clean)
            {
                continue;
            }
            else
            {
                throw AccountStoreException(
                    "Commit called with unsupported entry state: " +
                    std::to_string(static_cast<int>(entry.state))
                );
            }
        }
    }

    /**
     * @brief Update the active profile for the store, this will determine which
     * accounts are loaded and managed in the store, and should be called
     * whenever the active profile changes to ensure that the store is managing
     * the correct set of accounts for the current profile.
     *
     * @param profileIdOpt An optional containing the ID of the new active
     * profile, if std::nullopt is passed, it indicates that there is no active
     * profile, and the store should clear its data and not manage any accounts.
     */
    void AccountStore::updateActiveProfile(
        const std::optional<ProfileId>& profileIdOpt
    )
    {
        if (!profileIdOpt.has_value())
        {
            LOG_WARNING(
                "No active profile set, ignoring updateActiveProfile call"
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
    }

    /**
     * @brief Add a new cash account based on the given account draft
     *
     * @param accountDraft A draft containing the necessary information to
     * create a new cash account, this allows the store to take the user
     * input and convert it into a format that can be used to create a new
     * cash account in the underlying service, and ensures that the store
     * can validate and process the input before attempting to create the
     * account.
     * @return AccountStoreResult The result of the add operation, this
     * allows the caller to understand whether the account was added
     * successfully or if there was an error, and provides information about
     * what went wrong if the addition failed.
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

    /**
     * @brief Refresh the store's data by clearing existing entries and loading
     * accounts from the underlying service for the active profile, this should
     * be called whenever the active profile changes or when the store needs to
     * ensure that it has the most up-to-date data from the service, and will
     * repopulate the store with accounts that are relevant to the current
     * active profile.
     *
     */
    void AccountStore::_refresh()
    {
        _clearEntries();

        const auto accounts = _accountService->getAllAccounts(_activeProfileId);

        for (const auto& account : accounts)
            _addEntry(account, StoreState::Clean);
    }

    /**
     * @brief Retrieves a vector of pointers to all accounts currently in the
     * store, this allows callers to access the accounts managed by the store,
     * and provides a way to retrieve the account data for display or further
     * processing.
     *
     * @return std::vector<const finance::Account*> A vector of pointers to all
     * accounts currently in the store, each pointer points to an account object
     * that is managed by the store, and the caller can use these pointers to
     * access the account data and perform operations on the accounts as needed.
     */
    [[nodiscard]] std::vector<const finance::Account*> AccountStore::
        getAllAccounts() const
    {
        std::vector<const finance::Account*> accounts;

        for (const auto& entry : _getEntries())
        {
            if (std::holds_alternative<finance::CashAccount>(entry.value))
            {
                accounts.push_back(&std::get<finance::CashAccount>(entry.value)
                );
            }
            else
            {
                LOG_WARNING(
                    "Encountered unsupported account type in getAllAccounts, "
                    "skipping"
                );
            }
        }

        return accounts;
    }

}   // namespace app