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
    /**
     * @brief Detail struct for open stock positions, containing the position
     * draft, ticker, and position state information.
     *
     */
    struct OpenStockPositionDetail
    {
        /// The position draft containing the details of the open stock position
        drafts::PositionStockDetailDraft positionDraft;
        /// The ticker symbol of the stock associated with the open position
        std::string ticker;
        /// The position state containing the price-independent fold result
        finance::PositionState state;
    };

    /**
     * @brief Detail struct for open option positions, containing the position
     * draft, ticker, and position state information.
     *
     */
    struct OpenOptionPositionDetail
    {
        /// The position draft containing the details of the open option
        /// position
        drafts::PositionOptionDetailDraft positionDraft;
        /// The ticker symbol of the stock associated with the open option
        std::string ticker;
        /// The position state containing the price-independent fold result
        finance::PositionState state;
    };

    /**
     * @brief The PositionGateway class provides methods to interact with
     * position-related data, including retrieving open position transactions,
     * calculating profit and loss (PnL) for positions, and obtaining details of
     * open stock and option positions. It serves as a gateway between the
     * application layer and the underlying data stores for positions, options,
     * stocks, and transactions.
     *
     */
    class PositionGateway
    {
       private:
        /// The transaction store used to retrieve transaction data
        std::shared_ptr<store::ITransactionStore> _transactionStore;
        /// The position store used to retrieve position data
        std::shared_ptr<store::IPositionStore> _positionStore;
        /// The option store used to retrieve option data
        std::shared_ptr<store::IOptionStore> _optionStore;
        /// The stock store used to retrieve stock data
        std::shared_ptr<store::IStockStore> _stockStore;

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