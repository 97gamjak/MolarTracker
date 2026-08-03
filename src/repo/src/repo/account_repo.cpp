#include "account_repo.hpp"

#include "factories/account_factory.hpp"
#include "finance/account/account.hpp"
#include "logging/log_macros.hpp"
#include "orm/crud.hpp"
#include "orm/query_options.hpp"
#include "sql_models/account_row.hpp"

REGISTER_LOG_CATEGORY("Repo.AccountRepo");

namespace repo
{

    /**
     * @brief Create a new account in the repository
     *
     * This method takes an Account domain object as input and creates a
     * corresponding entry in the database, it returns the ID of the newly
     * created account
     *
     * @param account The Account domain object containing the details of the
     * account to be created
     * @param profileId The ID of the profile to which the account belongs
     *
     * @return CrudResult<AccountId> The ID of the newly created account
     */
    CrudResult<AccountId> AccountRepo::createAccount(
        const finance::Account& account,
        const ProfileId&        profileId
    )
    {
        const auto result = _getCrud().insert(
            _getDb(),
            AccountFactory::toAccountRow(account, profileId)
        );

        if (!result.has_value())
        {
            return result.error().convert(
                "Failed to create account with name '" + account.getName() +
                "' for profile with ID '" + profileId.toString() + "'"
            );
        }

        const auto accountId = AccountId(result.value());

        switch (account.getKind())
        {
            case AccountKind::Cash:
            {
                const auto cashResult = _getCrud().insert(
                    _getDb(),
                    AccountFactory::toCashAccountDetailRow(
                        std::get<finance::CashAccount>(account.getDetails()),
                        accountId
                    )
                );

                if (!cashResult.has_value())
                {
                    return cashResult.error().convert(
                        "Failed to create cash account details for account "
                        "with "
                        "name '" +
                        account.getName() + "' for profile with ID '" +
                        profileId.toString() + "'"
                    );
                }
                break;
            }
            case AccountKind::Security:
            case AccountKind::External:
                break;
        }

        return accountId;
    }

    /**
     * @brief Get the All Accounts
     *
     * @param profileId
     * @return std::vector<finance::Account>
     */
    std::vector<finance::Account> AccountRepo::getAllAccounts(
        const ProfileId& profileId
    )
    {
        auto query = orm::Query{}.where(AccountRow::hasProfileId(profileId));

        const auto accountRows = _getCrud().get<AccountRow>(_getDb(), query);

        auto accounts = AccountFactory::toAccountDomains(accountRows);

        for (auto& account : accounts)
        {
            LOG_DEBUG(
                "Retrieved account with ID '" + account.getId().toString() +
                "' for profile with ID '" + profileId.toString() + "'"
            );

            switch (account.getKind())
            {
                case AccountKind::Cash:
                {
                    const auto cashAccountRow =
                        _getCrud().getUnique<CashAccountDetailRow>(
                            _getDb(),
                            orm::Query{}.where(
                                CashAccountDetailRow::hasId(account.getId())
                            )
                        );

                    if (!cashAccountRow.has_value())
                    {
                        LOG_ERROR(
                            "Failed to retrieve cash account details for "
                            "account "
                            "with ID '" +
                            account.getId().toString() +
                            "' for profile with ID '" + profileId.toString() +
                            "'"
                        );

                        continue;
                    }

                    if (cashAccountRow.value().securityId.value().has_value())
                    {
                        account.setLinkedSecurityAccountId(
                            cashAccountRow.value().securityId.value().value()
                        );
                    }
                    break;
                }
                case AccountKind::Security:
                case AccountKind::External:
                    break;
            }
        }

        return accounts;
    }

    /**
     * @brief Check if an account exists in the repository
     *
     * This method checks if an account with the given ID exists in the
     * database. It returns true if the account exists, false otherwise.
     *
     * @param accountId The ID of the account to check for existence
     * @return bool True if the account exists, false otherwise
     */
    bool AccountRepo::accountExists(const AccountId& accountId)
    {
        const auto result = _getCrud().getUnique<AccountRow>(
            _getDb(),
            orm::Query{}.where(AccountRow::hasId(accountId))
        );

        return result.has_value();
    }

}   // namespace repo
