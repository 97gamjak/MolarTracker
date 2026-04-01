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
         * @brief Create a new cash account in the repository, this method takes
         * a CashAccount domain object as input and creates a corresponding
         * entry in the database, it returns the ID of the newly created cash
         * account
         *
         * @param cashAccount The CashAccount domain object containing the
         * details of the cash account to be created
         * @param profileId The ID of the profile to which the cash account
         * belongs
         * @return AccountId The ID of the newly created cash account
         */
        [[nodiscard]] virtual AccountId createCashAccount(
            const finance::Account& cashAccount,
            const ProfileId&        profileId
        ) = 0;

        /**
         * @brief Get all cash accounts from the repository, this method
         * retrieves all cash account entries from the database and returns
         them
         * as a vector of CashAccount domain objects
         *
         * @param profileId The ID of the profile whose cash accounts are to
         be
         * retrieved
         * @return std::vector<finance::Account> A vector containing all
         * cash accounts in the repository
         */
        [[nodiscard]] virtual std::vector<finance::Account> getAllCashAccounts(
            const ProfileId& profileId
        ) const = 0;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__REPOS_API__I_ACCOUNT_REPO_HPP__