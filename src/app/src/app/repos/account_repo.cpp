#include "account_repo.hpp"

#include "app/factories/account_factory.hpp"
#include "filter/operators.hpp"
#include "finance/account.hpp"
#include "logging/log_macros.hpp"
#include "orm/crud.hpp"
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
        auto accountRow = AccountFactory::toAccountRow(account, profileId);

        db::Transaction transaction{_getDb()};

        const auto result = orm::Crud().insert(_getDb(), accountRow);

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
    ) const
    {
        const auto where = orm::makeWhere(
            AccountRow::profileIdField{profileId},
            AccountRow::tableName,
            filter::Operator::Equal
        );

        const auto accountRows =
            orm::Crud().getAll<AccountRow>(_getDb(), where);

        return AccountFactory::toAccountDomains(accountRows);
    }

}   // namespace app