#include "account_repo.hpp"

#include "orm/crud.hpp"
#include "sql_models/account_row.hpp"

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
    AccountId AccountRepo::createCashAccount(const CashAccount& account)
    {
        const auto accountId =
            orm::insert(_db, AccountFactory::toAccountRow(account));
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