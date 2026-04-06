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
         * @brief Create a new account, this method takes a Account
         * domain object as input and creates a corresponding entry in the
         * database, it returns the ID of the newly created account.
         *
         * @param account The Account domain object containing the
         * details of the account to be created
         * @param profileId The ID of the profile to which the account
         * belongs
         *
         * @return AccountId The ID of the newly created account
         */
        [[nodiscard]]
        virtual AccountId createAccount(
            const finance::Account& account,
            const ProfileId&        profileId
        ) = 0;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__SERVICES_API__I_ACCOUNT_SERVICE_HPP__