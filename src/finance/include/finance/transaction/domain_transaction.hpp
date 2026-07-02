#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__DOMAIN_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__DOMAIN_TRANSACTION_HPP__

#include <optional>
#include <string>

#include "config/id_types.hpp"
#include "finance/transaction/transaction.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "transaction_data.hpp"
#include "transaction_entry.hpp"
#include "utils/timestamp.hpp"

namespace finance
{
    /**
     * @brief A class representing a financial transaction, which may involve
     * multiple accounts and cash movements. This class serves as a base for
     * more specific transaction types (e.g., deposits, withdrawals, transfers).
     *
     */
    class DomainTransaction : public BaseTransaction
    {
       private:
        /// The data associated with the transaction
        TransactionData _data;

        /// A container for all entries associated with the transaction
        TransactionEntries _entries;

       public:
        explicit DomainTransaction(
            TransactionId              id,
            Timestamp                  timestamp,
            TransactionStatus          status,
            TransactionData            data,
            TransactionEntries         entries,
            std::optional<std::string> comment = std::nullopt
        );

        [[nodiscard]] TransactionDataType    getType() const;
        [[nodiscard]] const TransactionData& getData() const;

        [[nodiscard]] bool hasPositionId(PositionId id) const;
        [[nodiscard]] bool hasInstrumentId(InstrumentId id) const;

        [[nodiscard]] const TransactionEntries& getEntries() const;
        void addEntry(const TransactionEntry& entry);
        void setEntries(const TransactionEntries& entries);

        [[nodiscard]] const TradeLegs& getLegs() const;
        void                           addLeg(const TradeLeg& leg);
        void                           setLegs(const TradeLegs& legs);

        [[nodiscard]] std::string toString() const override;
    };

    bool hasPositionId(const DomainTransaction& transaction, PositionId id);

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__DOMAIN_TRANSACTION_HPP__
