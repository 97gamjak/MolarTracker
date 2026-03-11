#ifndef __APP__SERVICES_API__I_ACCOUNT_SERVICE_HPP__
#define __APP__SERVICES_API__I_ACCOUNT_SERVICE_HPP__

#include <vector>

#include "config/id_types.hpp"

namespace finance
{
    class CashAccount;   // forward declaration
}   // namespace finance

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
         * @brief Get all cash accounts, this method retrieves all cash accounts
         * from the underlying data source and returns them as a vector of
         * CashAccount domain objects.
         *
         * @return std::vector<finance::CashAccount> A vector containing all
         * cash accounts
         */
        [[nodiscard]] virtual std::vector<finance::CashAccount> getAllCashAccounts(
        ) const = 0;

        /**
         * @brief Create a new cash account, this method takes a CashAccount
         * domain object as input and creates a corresponding entry in the
         * database, it returns the ID of the newly created cash account.
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

#endif   // __APP__SERVICES_API__I_ACCOUNT_SERVICE_HPP__