#include "account_repo.hpp"

#include "app/factories/account_factory.hpp"
#include "finance/account.hpp"
#include "logging/log_macros.hpp"
#include "orm/crud.hpp"
#include "orm/query_options.hpp"
#include "repo_errors.hpp"
#include "sql_models/account_row.hpp"

REGISTER_LOG_CATEGORY("App.Repo.AccountRepo");

namespace app
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
     * @return AccountId The ID of the newly created account
     */
    AccountId AccountRepo::createAccount(
        const finance::Account& account,
        const ProfileId&        profileId
    )
    {
        db::Transaction transaction{_getDb()};

        const auto result = _getCrud().insert(
            _getDb(),
            AccountFactory::toAccountRow(account, profileId)
        );

        if (!result.has_value())
        {
            transaction.rollback();

            const auto whatFailed = "account entry for account with name '" +
                                    account.getName() + "'";

            const auto msg = getInsertError(result.error(), whatFailed);

            LOG_ERROR(msg);
            throw orm::CrudException(msg);
        }

        transaction.commit();

        return AccountId(result.value());
    }

    /**
     * @brief Get the All Accounts
     *
     * @param profileId
     * @return std::vector<finance::Account>
     */
    [[nodiscard]] std::vector<finance::Account> AccountRepo::getAllAccounts(
        const ProfileId& profileId
    )
    {
        auto query =
            orm::QueryOptions{}.where(AccountRow::hasProfileId(profileId));

        const auto accountRows = _getCrud().get<AccountRow>(_getDb(), query);

        return AccountFactory::toAccountDomains(accountRows);
    }

}   // namespace app