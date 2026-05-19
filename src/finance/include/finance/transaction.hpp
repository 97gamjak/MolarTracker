#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION_HPP__

#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "config/strong_id.hpp"
#include "finance/cash.hpp"
#include "finance/trade_data.hpp"
#include "transaction_entry.hpp"
#include "utils/timestamp.hpp"

namespace finance
{
    struct CashData
    {
    };

    using TransactionData = std::variant<CashData, TradeData>;

    /**
     * @brief A class representing a financial transaction, which may involve
     * multiple accounts and cash movements. This class serves as a base for
     * more specific transaction types (e.g., deposits, withdrawals, transfers).
     *
     */
    class Transaction
    {
       private:
        /// The unique identifier for the transaction
        TransactionId _id;

        /// The timestamp when the transaction was created
        Timestamp _timestamp;

        /// The status of the transaction (e.g., completed, deleted)
        TransactionStatus _status;

        /// The data associated with the transaction
        TransactionData _data;

        /// A list of entries associated with the transaction, each entry
        /// represents a specific cash movement or account change related to the
        /// transaction
        std::vector<TransactionEntry> _entries;

        /// An optional comment or description for the transaction
        std::optional<std::string> _comment;

       public:
        explicit Transaction(
            TransactionId                 id,
            Timestamp                     timestamp,
            TransactionStatus             status,
            TransactionData               data,
            std::vector<TransactionEntry> entries,
            std::optional<std::string>    comment = std::nullopt
        );

        [[nodiscard]] TransactionId                        getId() const;
        [[nodiscard]] Timestamp                            getTimestamp() const;
        [[nodiscard]] TransactionStatus                    getStatus() const;
        [[nodiscard]] std::optional<std::string>           getComment() const;
        [[nodiscard]] const std::vector<TransactionEntry>& getEntries() const;
        [[nodiscard]] std::vector<TransactionEntry>&       getEntries();
        [[nodiscard]] TransactionDataType                  getType() const;
        [[nodiscard]] const TransactionData&               getData() const;
        [[nodiscard]] TransactionData&                     getData();
        [[nodiscard]] std::vector<InstrumentId> getInstrumentIds() const;

        [[nodiscard]] Cash calculateTotalSum() const;

        void setId(TransactionId id);
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
#include "transaction.tpp"
#endif

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION_HPP__
