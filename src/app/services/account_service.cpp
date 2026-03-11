#include "account_service.hpp"

#include "app/repos_api/i_account_repo.hpp"

namespace app
{

    /**
     * @brief Construct a new Account Service:: Account Service object
     *
     * @param accountRepo
     */
    AccountService::AccountService(std::shared_ptr<IAccountRepo> accountRepo)
        : _accountRepo(std::move(accountRepo))
    {
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