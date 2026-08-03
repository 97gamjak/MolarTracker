#ifndef __REPO__INCLUDE__REPO__I_ACCOUNT_REPO_HPP__
#define __REPO__INCLUDE__REPO__I_ACCOUNT_REPO_HPP__

#include <vector>

#include "config/id_types.hpp"
#include "error/crud_error.hpp"

namespace finance
{
    class Account;   // forward declaration
}   // namespace finance

namespace repo
{

    /**
     * @brief Interface for Account repository
     *
     */
    class IAccountRepo
    {
       public:
        virtual ~IAccountRepo() = default;

        /**
         * @brief Create a new account in the repository, this method takes
         * an Account domain object as input and creates a corresponding
         * entry in the database, it returns the ID of the newly created account
         *
         * @param account The Account domain object containing the
         * details of the account to be created
         * @param profileId The ID of the profile to which the account
         * belongs
         * @return CrudResult<AccountId> The ID of the newly created account, or
         * an error if the operation failed
         */
        [[nodiscard]]
        virtual CrudResult<AccountId> createAccount(
            const finance::Account& account,
            const ProfileId&        profileId
        ) = 0;

        /**
         * @brief Get all accounts from the repository, this method
         * retrieves all account entries from the database and returns
         * them as a vector of Account domain objects
         *
         * @param profileId The ID of the profile whose accounts are to
         * be retrieved
         * @return std::vector<finance::Account> A vector containing all
         * accounts in the repository
         */
        [[nodiscard]]
        virtual std::vector<finance::Account> getAllAccounts(
            const ProfileId& profileId
        ) = 0;

        /**
         * @brief Check if an account exists in the repository, this method
         * checks if an account with the given ID exists in the database
         *
         * @param accountId The ID of the account to check for existence
         * @return true If the account exists in the repository
         * @return false If the account does not exist in the repository
         */
        [[nodiscard]]
        virtual bool accountExists(const AccountId& accountId) = 0;
    };

}   // namespace repo

#endif   // __REPO__INCLUDE__REPO__I_ACCOUNT_REPO_HPP__
