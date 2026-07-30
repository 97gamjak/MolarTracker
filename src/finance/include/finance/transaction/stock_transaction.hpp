#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__STOCK_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__STOCK_TRANSACTION_HPP__

#include "common/cash.hpp"
#include "common/quantity.hpp"
#include "config/id_types.hpp"
#include "finance/transaction/security_transaction.hpp"
#include "finance/transaction/stock_data.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "finance/transaction/transaction_entry.hpp"

namespace finance
{
    /**
     * @brief A stock transaction represents a financial transaction involving
     * stocks.
     *
     */
    class StockTransaction : public SecurityTransaction
    {
       private:
        /// The unit price of the stock being traded in the transaction
        Cash _unitPrice;

       public:
        StockTransaction(
            TransactionId              id,
            Timestamp                  timestamp,
            TransactionStatus          status,
            InstrumentId               instrumentId,
            AccountId                  securityAccount,
            AccountId                  cashAccount,
            AccountId                  externalAccount,
            Quantity                   quantity,
            Cash                       unitPrice,
            Cash                       fees,
            PositionId                 positionId,
            std::optional<std::string> comment = std::nullopt
        );

        [[nodiscard]] const Cash& getUnitPrice() const;

        [[nodiscard]]
        TransactionEntries getEntries(AccountId externalAccount) const override;

        [[nodiscard]] StockData getStockData() const;

        [[nodiscard]] Cash getAmount() const;

       private:
        [[nodiscard]] TransactionEntry _getAmountEntry() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__STOCK_TRANSACTION_HPP__
