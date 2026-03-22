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

        MT_DEBUG << "Committing changes in AccountStore, number of entries: "
                 << _getEntries().size();

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

                [[maybe_unused]] auto result =
                    _accountService->createCashAccount(
                        std::get<finance::CashAccount>(entry.value),
                        _activeProfileId
                    );
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
            _refresh();
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
        _refresh();
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

    void AccountStore::_refresh()
    {
        _clearEntries();

        const auto accounts = _accountService->getAllAccounts(_activeProfileId);

        for (const auto& account : accounts)
            _addEntry(account, StoreState::Clean);
    }

}   // namespace app