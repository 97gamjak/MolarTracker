#ifndef __CONTROLLER__SRC__CONTROLLER__MAPPER__TRANSACTION__TRANSACTION_OVERVIEW_MAPPER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MAPPER__TRANSACTION__TRANSACTION_OVERVIEW_MAPPER_HPP__

#include <memory>
#include <vector>

#include "drafts/transaction/transaction_overview_draft.hpp"
#include "error/finance_error.hpp"

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
        static FinanceResult<std::vector<drafts::StockTransactionOverview>> toStockOverview(
            const finance::Transactions&                transactions,
            const std::shared_ptr<cache::StockCache>&   stockCache,
            const std::shared_ptr<cache::AccountCache>& accountCache
        );

        static FinanceResult<std::vector<drafts::CashTransactionOverview>> toCash(
            const finance::Transactions&                transactions,
            const std::shared_ptr<cache::AccountCache>& accountCache
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MAPPER__TRANSACTION__TRANSACTION_OVERVIEW_MAPPER_HPP__
