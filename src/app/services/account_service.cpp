#include "account_service.hpp"

#include "app/repos_api/i_account_repo.hpp"
#include "finance/cash_account.hpp"

namespace app
{

    /**
     * @brief Construct a new Account Service:: Account Service object
     *
     * @param accountRepo
     */
    AccountService::AccountService(
        const std::shared_ptr<IAccountRepo>& accountRepo
    )
        : _accountRepo(accountRepo)
    {
    }

    /**
     * @brief Get all cash accounts
     *
     * @return std::vector<finance::CashAccount>
     */
    std::vector<finance::CashAccount> AccountService::getAllCashAccounts() const
    {
        return _accountRepo->getAllCashAccounts();
    }

    /**
     * @brief Create a new cash account
     *
     * @param cashAccount
     * @return AccountId
     */
    AccountId AccountService::createCashAccount(
        const finance::CashAccount& cashAccount
    )
    {
        return _accountRepo->createCashAccount(cashAccount);
    }

}   // namespace app