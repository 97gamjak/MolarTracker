#ifndef __SERVICE__INCLUDE__SERVICE__I_ACCOUNT_SERVICE_HPP__
#define __SERVICE__INCLUDE__SERVICE__I_ACCOUNT_SERVICE_HPP__

#include <vector>

#include "config/id_types.hpp"
#include "error/crud_error.hpp"
#include "finance/account/account.hpp"

namespace service
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
         * @return CrudResult<AccountId> The result of the create operation
         */
        [[nodiscard]]
        virtual CrudResult<AccountId> createAccount(
            const finance::Account& account,
            const ProfileId&        profileId
        ) = 0;

        /**
         * @brief Update an existing account, this method takes a Account
         * domain object as input and updates the corresponding entry in the
         * database. It returns a CrudResult indicating the success or failure
         * of the operation.
         *
         * @param account The Account domain object containing the updated
         * details of the account to be updated
         * @param profileId The ID of the profile to which the account belongs
         *
         * @return CrudResult<void> A result indicating success or failure of
         * the update operation
         */
        [[nodiscard]]
        virtual CrudResult<void> updateAccount(
            const finance::Account& account,
            const ProfileId&        profileId
        ) = 0;

        /**
         * @brief Delete an existing account, this method removes the account
         * with the given ID from the database. It returns a CrudResult
         * indicating the success or failure of the operation.
         *
         * @param accountId The ID of the account to delete
         *
         * @return CrudResult<void> A result indicating success or failure of
         * the delete operation
         */
        [[nodiscard]]
        virtual CrudResult<void> deleteAccount(const AccountId& accountId) = 0;
    };

}   // namespace service

#endif   // __SERVICE__INCLUDE__SERVICE__I_ACCOUNT_SERVICE_HPP__
