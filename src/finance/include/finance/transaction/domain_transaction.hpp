#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION_HPP__

#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "config/strong_id.hpp"
#include "finance/cash.hpp"
#include "finance/transaction/transaction.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "trade_data.hpp"
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
    class DomainTransaction : public Transaction
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

        [[nodiscard]] const TransactionEntries& getEntries() const;
        [[nodiscard]] TransactionEntries&       getEntries();
        [[nodiscard]] TransactionDataType       getType() const;
        [[nodiscard]] const TransactionData&    getData() const;
        [[nodiscard]] TransactionData&          getData();
        [[nodiscard]] std::vector<InstrumentId> getInstrumentIds() const;

        [[nodiscard]] Cash     calculateTotalSum() const;
        [[nodiscard]] Quantity calculateTotalQuantity() const;

        void addEntry(const TransactionEntry& entry);
        void addLeg(const TradeLeg& leg);

        [[nodiscard]] std::vector<TradeLeg> getLegs() const;

        [[nodiscard]] std::string toString() const;
    };

    /**
     * @brief Base visitor for extracting IDs from transaction data
     *
     * @tparam IdType
     */
    template <typename IdType>
    struct GetIdVisitorBase
    {
        std::vector<IdType> operator()(const CashData& /*cashData*/) const;

       protected:
        template <typename Proj>
        static std::vector<IdType> fromLegs(
            const TradeData& tradeData,
            Proj             proj
        );
    };

    /**
     * @brief Visitor for extracting IDs from trade data
     *
     * @tparam IdType
     * @tparam Proj
     */
    template <typename IdType, typename Proj>
    struct GetIdVisitor : GetIdVisitorBase<IdType>
    {
        /// The projection function to get the ID from a leg
        Proj proj;

        /// The projection function to set the ID on a leg
        using GetIdVisitorBase<IdType>::operator();

        explicit GetIdVisitor(Proj _proj);

        std::vector<IdType> operator()(const TradeData& tradeData) const;
    };

    template <typename IdType, typename Proj>
    bool hasId(
        const TransactionData&                data,
        const unorderedIdMap<IdType, IdType>& map,
        Proj                                  proj
    );

    template <typename IdType, typename Proj>
    bool hasId(const TransactionData& data, IdType id, Proj proj);

}   // namespace finance

#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION_TPP__
#include "domain_transaction.tpp"
#endif

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION_HPP__
