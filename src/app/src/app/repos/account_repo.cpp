#include "account_repo.hpp"

#include "app/factories/account_factory.hpp"
#include "finance/account.hpp"
#include "logging/log_macros.hpp"
#include "orm/crud.hpp"
#include "repo_errors.hpp"
#include "sql_models/account_row.hpp"

REGISTER_LOG_CATEGORY("App.Repo.AccountRepo");

namespace app
{

    /**
     * @brief Construct a new Account Repo object
     *
     * @param db A reference to the database instance that the repository
     * will use to perform database operations, this allows the repository to
     * interact with the database to store and retrieve account data as needed.
     */
    AccountRepo::AccountRepo(db::Database& db) : _db(db) {}

    /**
     * @brief Create a new cash account in the database, this method should
     * insert a new row into the accounts table and a corresponding row into the
     * cash_accounts table to create a new cash account, it should return the ID
     * of the newly created account on success, or throw an exception if the
     * operation fails for any reason (e.g., database error, validation error,
     * etc.).
     *
     * @param account The CashAccount object containing the details of the cash
     * account to create, this should include all necessary information such as
     * the account name, initial balance, etc., and the method should validate
     * this information before attempting to create the account in the database.
     * @param profileId The ID of the profile to which the cash account belongs,
     * this is necessary to associate the account with the correct user profile
     * in the database, and should be validated to ensure that it corresponds to
     * an existing profile before creating the account.
     *
     * @return AccountId The ID of the newly created cash account, this can be
     * used to retrieve or manage the account later on, if the account creation
     * fails, the method should throw an exception with details about the
     * failure.
     */
    AccountId AccountRepo::createCashAccount(
        const finance::Account& account,
        const ProfileId&        profileId
    )
    {
        auto [accountRow, cashAccountRowVar] =
            AccountFactory::toAccountRow(account, profileId);

        auto cashAccountRow = std::get<CashAccountRow>(cashAccountRowVar);

        db::Transaction transaction{_db};

        const auto result = orm::Crud().insert(_db, transaction, accountRow);

        if (!result.has_value())
        {
            transaction.rollback();

            const auto whatFailed =
                "account entry for cash account with name '" +
                account.getName() + "'";

            const auto msg = getInsertError(result.error(), whatFailed);

            LOG_ERROR(msg);
            throw orm::CrudException(msg);
        }

        cashAccountRow.id = AccountId(result.value());

        const auto cashResult =
            orm::Crud().insert(_db, transaction, cashAccountRow);

        if (!cashResult.has_value())
        {
            transaction.rollback();

            const auto whatFailed = "cash account details for account ID " +
                                    accountRow.id.value().toString() +
                                    " with name '" + account.getName() + "'";

            const auto msg = getInsertError(cashResult.error(), whatFailed);

            LOG_ERROR(msg);
            throw orm::CrudException(msg);
        }

        transaction.commit();

        return cashAccountRow.id.value();
    }

    /**
     * @brief Get the All Cash Accounts
     *
     * @details This implementation at the moment works only because the
     * CashAccountRow does not contain any additional fields yet
     *
     * @param profileId
     * @return std::vector<finance::CashAccount>
     */
    [[nodiscard]] std::vector<finance::Account> AccountRepo::getAllCashAccounts(
        const ProfileId& profileId
    ) const
    {
        auto dummyAccountRow      = AccountRow{};
        dummyAccountRow.kind      = AccountKind::Cash;
        dummyAccountRow.profileId = profileId;

        const auto kindClause = orm::WhereClause(
            dummyAccountRow.kind,
            AccountRow::tableName,
            orm::WhereOperator::Equal
        );
        const auto profileClause = orm::WhereClause(
            dummyAccountRow.profileId,
            AccountRow::tableName,
            orm::WhereOperator::Equal
        );

        const auto cashAccountRows = orm::Crud().getAll<AccountRow>(
            _db,
            orm::WhereClauses{kindClause, profileClause}
        );

        return AccountFactory::toAccountDomains(cashAccountRows);
    }

}   // namespace app