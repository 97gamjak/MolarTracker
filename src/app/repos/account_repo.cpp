#include "account_repo.hpp"

#include "app/factories/account_factory.hpp"
#include "finance/cash_account.hpp"
#include "logging/log_macros.hpp"
#include "orm/crud.hpp"
#include "orm/join.hpp"
#include "repo_errors.hpp"
#include "sql_models/account_row.hpp"

REGISTER_LOG_CATEGORY("App.Repo.AccountRepo");

namespace app
{

    /**
     * @brief Construct a new Account Repo object
     *
     * @param db A shared pointer to the database instance that the repository
     * will use to perform database operations, this allows the repository to
     * interact with the database to store and retrieve account data as needed.
     */
    AccountRepo::AccountRepo(const std::shared_ptr<db::Database>& db) : _db(db)
    {
    }

    /**
     * @brief Ensure the database schema for accounts exists, this method should
     * create the necessary tables and relationships in the database to store
     * and manage account data, it should be called before using any other
     * methods of the repository to ensure that the database is properly set up.
     *
     */
    void AccountRepo::ensureSchema() { _ensureSchema(); }

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
     *
     * @return AccountId The ID of the newly created cash account, this can be
     * used to retrieve or manage the account later on, if the account creation
     * fails, the method should throw an exception with details about the
     * failure.
     */
    AccountId AccountRepo::createCashAccount(
        const finance::CashAccount& account
    )
    {
        auto [accountRow, cashAccountRow] = CashAccountFactory::toRow(account);

        db::Transaction transaction{_db};

        const auto result = orm::insert(_db, transaction, accountRow);

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

        const auto cashResult = orm::insert(_db, transaction, cashAccountRow);

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

        return account.getId();
    }

    [[nodiscard]] std::vector<finance::CashAccount> AccountRepo::
        getAllCashAccounts() const
    {
        const auto baseName = CashAccountRow::tableName;
        const auto joinName = AccountRow::tableName;

        const auto fieldPair = std::make_pair(
            decltype(CashAccountRow{}.id)::getColumnName(),
            decltype(AccountRow{}.id)::getColumnName()
        );

        orm::Joins joins;
        joins.addJoin(orm::Join(baseName, joinName, fieldPair));

        auto dummyAccountRow = AccountRow{};
        dummyAccountRow.kind = AccountKind::Cash;

        const auto kindClause = orm::WhereClause(
            dummyAccountRow.kind,
            AccountRow::tableName,
            orm::WhereOperator::Equal
        );

        const auto cashAccountRows = orm::getAll<CashAccountRow>(
            _db,
            joins,
            orm::WhereClauses{kindClause}
        );

        return CashAccountFactory::toDomains(cashAccountRows);
    }

    /**
     * @brief Ensure the database schema for accounts exists, this is a private
     * helper method that is called by the virtual overriding method
     * ensureSchema() to not use virtual dispatch in constructor, this method
     * should create the necessary tables and relationships in the database to
     * store and manage account data, it should be called before using any other
     * methods of the repository to ensure that the database is properly set up.
     *
     */
    void AccountRepo::_ensureSchema() const
    {
        orm::createTable<AccountRow>(_db);
    }

}   // namespace app