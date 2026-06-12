#ifndef __REPO__SRC__REPO__FACTORIES__TRANSACTION_FACTORY_HPP__
#define __REPO__SRC__REPO__FACTORIES__TRANSACTION_FACTORY_HPP__

#include "config/id_types.hpp"
#include "finance/transaction/transaction_data.hpp"
#include "orm/join.hpp"
#include "orm/where_expr.hpp"

namespace finance
{
    class DomainTransaction;   // forward declaration
    class TransactionEntry;    // forward declaration
    class TradeLeg;            // forward declaration
    class TransactionFilter;   // forward declaration
    class OptionData;          // forward declaration
}   // namespace finance

struct TransactionRow;         // forward declaration
struct TransactionEntryRow;    // forward declaration
struct TradeLegRow;            // forward declaration
struct TransactionOptionRow;   // forward declaration

namespace repo
{

    /**
     * @brief A factory class for creating and converting transaction-related
     * objects
     *
     */
    class TransactionFactory
    {
       public:
        [[nodiscard]]
        static TransactionRow toRow(
            const finance::DomainTransaction& transaction
        );

        [[nodiscard]]
        static finance::DomainTransaction fromCashRow(
            const TransactionRow& row
        );

        [[nodiscard]]
        static finance::DomainTransaction fromStockRow(
            const TransactionRow& row
        );

        [[nodiscard]]
        static finance::DomainTransaction fromOptionRow(
            const TransactionRow&       row,
            const TransactionOptionRow& optionRow
        );

        [[nodiscard]]
        static TransactionEntryRow toEntryRow(
            const finance::TransactionEntry& entry,
            TransactionId                    transactionId
        );

        [[nodiscard]]
        static finance::TransactionEntry fromEntryRow(
            const TransactionEntryRow& row
        );

        [[nodiscard]]
        static TradeLegRow toLegRow(
            const finance::TradeLeg& leg,
            TransactionId            transactionId
        );

        [[nodiscard]]
        static finance::TradeLeg fromLegRow(const TradeLegRow& row);

        [[nodiscard]]
        static TransactionOptionRow toOptionRow(
            const finance::OptionData& optionData,
            TransactionId              transactionId
        );

        [[nodiscard]]
        static orm::WhereExpr toWhereExpr(
            const finance::TransactionFilter& filter
        );

        [[nodiscard]]
        static orm::Joins toJoin(const finance::TransactionFilter& filter);

       private:
        [[nodiscard]]
        static finance::DomainTransaction _fromRow(
            const TransactionRow&           row,
            const finance::TransactionData& transactionData
        );
    };

}   // namespace repo

#endif   // __REPO__SRC__REPO__FACTORIES__TRANSACTION_FACTORY_HPP__
