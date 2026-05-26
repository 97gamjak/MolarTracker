#ifndef __REPO__INCLUDE__REPO__I_ACCOUNT_REPO_HPP__
#define __REPO__INCLUDE__REPO__I_ACCOUNT_REPO_HPP__

#include <vector>

#include "config/id_types.hpp"

namespace domain
{
    class Account;   // forward declaration
}   // namespace domain

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
         * @return AccountId The ID of the newly created account
         */
        [[nodiscard]] virtual AccountId createAccount(
            const domain::Account& account,
            const ProfileId&       profileId
        ) = 0;

        /**
         * @brief Get all accounts from the repository, this method
         * retrieves all account entries from the database and returns
         * them as a vector of Account domain objects
         *
         * @param profileId The ID of the profile whose accounts are to
         * be retrieved
         * @return std::vector<domain::Account> A vector containing all
         * accounts in the repository
         */
        [[nodiscard]] virtual std::vector<domain::Account> getAllAccounts(
            const ProfileId& profileId
        ) = 0;
    };

}   // namespace repo

#endif   // __REPO__INCLUDE__REPO__I_ACCOUNT_REPO_HPP__