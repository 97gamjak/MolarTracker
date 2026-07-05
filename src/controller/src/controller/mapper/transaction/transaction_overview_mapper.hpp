#ifndef __CONTROLLER__SRC__CONTROLLER__MAPPER__TRANSACTION__TRANSACTION_OVERVIEW_MAPPER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MAPPER__TRANSACTION__TRANSACTION_OVERVIEW_MAPPER_HPP__

#include <memory>
#include <vector>

#include "config/error.hpp"
#include "drafts/transaction/transaction_overview_draft.hpp"

namespace finance
{
    class Transactions;   // Forward declaration
}   // namespace finance

namespace cache
{
    class StockCache;     // Forward declaration
    class AccountCache;   // Forward declaration
}   // namespace cache

namespace controller
{

    /**
     * @brief Maps transaction overviews from the finance domain to the drafts
     * domain.
     *
     */
    class TransactionOverviewMapper
    {
       public:
        static Result<std::vector<drafts::StockTransactionOverview>, FinanceError> toStockOverview(
            const finance::Transactions&                transactions,
            const std::shared_ptr<cache::StockCache>&   stockCache,
            const std::shared_ptr<cache::AccountCache>& accountCache
        );

        static Result<std::vector<drafts::CashTransactionOverview>, FinanceError> toCash(
            const finance::Transactions&                transactions,
            const std::shared_ptr<cache::AccountCache>& accountCache
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MAPPER__TRANSACTION__TRANSACTION_OVERVIEW_MAPPER_HPP__
