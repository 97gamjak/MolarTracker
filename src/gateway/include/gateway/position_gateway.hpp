#ifndef __GATEWAY__INCLUDE__GATEWAY__POSITION_GATEWAY_HPP__
#define __GATEWAY__INCLUDE__GATEWAY__POSITION_GATEWAY_HPP__

#include <memory>

#include "drafts/position/position_option_draft.hpp"
#include "drafts/position/position_stock_draft.hpp"
#include "finance/position.hpp"
#include "finance/transaction/pnl.hpp"
#include "finance/transaction/transactions.hpp"   // for return value

namespace store
{
    class ITransactionStore;   // forward declaration
    class IPositionStore;      // forward declaration
    class IOptionStore;        // forward declaration
    class IStockStore;         // forward declaration
}   // namespace store

namespace gateway
{
    struct OpenStockPositionDetail
    {
        drafts::PositionStockDetailDraft positionDraft;
        std::string                      ticker;
        finance::PositionState state;   // price-independent fold result
    };

    /**
     * @brief Detail struct for open option positions, containing the position
     * draft, ticker, and PnL information.
     *
     */
    struct OpenOptionPositionDetail
    {
        drafts::PositionOptionDetailDraft positionDraft;
        std::string                       ticker;
        finance::PositionState state;   // price-independent fold result
    };

    class PositionGateway
    {
       private:
        std::shared_ptr<store::ITransactionStore> _transactionStore;
        std::shared_ptr<store::IPositionStore>    _positionStore;
        std::shared_ptr<store::IOptionStore>      _optionStore;
        std::shared_ptr<store::IStockStore>       _stockStore;

       public:
        PositionGateway(
            const std::shared_ptr<store::ITransactionStore>& transactionStore,
            const std::shared_ptr<store::IPositionStore>&    positionStore,
            const std::shared_ptr<store::IOptionStore>&      optionStore,
            const std::shared_ptr<store::IStockStore>&       stockStore
        );

        [[nodiscard]]
        FinanceResult<std::vector<std::pair<
            finance::Position,
            finance::
                Transactions>>> getOpenPositionTransactions(const IdSet<AccountId>& accountIds) const;

        [[nodiscard]]
        PnLResult<finance::PositionPnl> calculatePositionPnl(
            const finance::Transactions& positionTxs,
            std::optional<Cash>          markPrice
        ) const;

        [[nodiscard]]
        FinanceResult<std::vector<OpenStockPositionDetail>> getOpenStockPositionDetails(
            AccountId account
        ) const;

        [[nodiscard]]
        FinanceResult<std::vector<drafts::PositionStockDetailDraft>> getOpenStockPosition(
            AccountId account
        ) const;

        [[nodiscard]]
        FinanceResult<std::vector<OpenOptionPositionDetail>> getOpenOptionPositionDetails(
            AccountId account
        ) const;
    };
}   // namespace gateway

#endif   // __GATEWAY__INCLUDE__GATEWAY__POSITION_GATEWAY_HPP__