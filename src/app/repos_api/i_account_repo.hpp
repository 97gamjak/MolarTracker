#ifndef __APP__REPOS_API__I_ACCOUNT_REPO_HPP__
#define __APP__REPOS_API__I_ACCOUNT_REPO_HPP__

#include "config/id_types.hpp"

namespace finance
{
    class CashAccount;   // forward declaration
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
         * @brief Ensure that the necessary database schema for the Account
         * repository exists, this should be called before using any other
         * methods of the repository to ensure that the database is properly set
         *
         */
        virtual void ensureSchema() = 0;

        /**
         * @brief Create a new cash account in the repository, this method takes
         * a CashAccount domain object as input and creates a corresponding
         * entry in the database, it returns the ID of the newly created cash
         * account
         *
         * @param cashAccount The CashAccount domain object containing the
         * details of the cash account to be created
         * @return AccountId The ID of the newly created cash account
         */
        [[nodiscard]]
        virtual AccountId createCashAccount(
            const finance::CashAccount& cashAccount
        ) = 0;
    };

}   // namespace app

#endif   // __APP__REPOS_API__I_ACCOUNT_REPO_HPP__