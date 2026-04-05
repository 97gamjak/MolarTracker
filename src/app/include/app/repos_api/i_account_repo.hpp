#ifndef __APP__INCLUDE__APP__REPOS_API__I_ACCOUNT_REPO_HPP__
#define __APP__INCLUDE__APP__REPOS_API__I_ACCOUNT_REPO_HPP__

#include <vector>

#include "config/id_types.hpp"

namespace finance
{
    class Account;   // forward declaration
}   // namespace finance

namespace app
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
        [[nodiscard]] virtual std::vector<finance::Account> getAllAccounts(
            const ProfileId& profileId
        ) const = 0;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__REPOS_API__I_ACCOUNT_REPO_HPP__