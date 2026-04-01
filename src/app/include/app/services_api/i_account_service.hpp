#ifndef __APP__INCLUDE__APP__SERVICES_API__I_ACCOUNT_SERVICE_HPP__
#define __APP__INCLUDE__APP__SERVICES_API__I_ACCOUNT_SERVICE_HPP__

#include <vector>

#include "config/id_types.hpp"
#include "finance/account.hpp"

namespace app
{
    /**
     * @brief Interface for Account service
     *
     */
    class IAccountService
    {
       public:
        virtual ~IAccountService() = default;

        /**
         * @brief Get all accounts
         *
         * @param profileId The ID of the profile whose accounts are to be
         * retrieved
         *
         * @return std::vector<finance::Account> A vector containing all
         * accounts
         */
        [[nodiscard]] virtual std::vector<finance::Account> getAllAccounts(
            const ProfileId& profileId
        ) const = 0;

        /**
         * @brief Get all cash accounts, this method retrieves all cash accounts
         * from the underlying data source and returns them as a vector of
         * CashAccount domain objects.
         *
         * @param profileId The ID of the profile whose cash accounts are to be
         * retrieved
         *
         * @return std::vector<finance::CashAccount> A vector containing all
         * cash accounts
         */
        [[nodiscard]] virtual std::vector<finance::Account> getAllCashAccounts(
            const ProfileId& profileId
        ) const = 0;

        /**
         * @brief Create a new cash account, this method takes a CashAccount
         * domain object as input and creates a corresponding entry in the
         * database, it returns the ID of the newly created cash account.
         *
         * @param cashAccount The CashAccount domain object containing the
         * details of the cash account to be created
         * @param profileId The ID of the profile to which the cash account
         * belongs
         *
         * @return AccountId The ID of the newly created cash account
         */
        [[nodiscard]]
        virtual AccountId createCashAccount(
            const finance::Account& cashAccount,
            const ProfileId&        profileId
        ) = 0;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__SERVICES_API__I_ACCOUNT_SERVICE_HPP__