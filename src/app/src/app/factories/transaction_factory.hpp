#ifndef __APP__SRC__APP__FACTORIES__TRANSACTION_FACTORY_HPP__
#define __APP__SRC__APP__FACTORIES__TRANSACTION_FACTORY_HPP__

#include "config/id_types.hpp"
#include "orm/where_expr.hpp"

namespace finance
{
    class Transaction;         // forward declaration
    class TransactionEntry;    // forward declaration
    class TradeLeg;            // forward declaration
    class TransactionFilter;   // forward declaration
}   // namespace finance

struct TransactionRow;        // forward declaration
struct TransactionEntryRow;   // forward declaration
struct TradeLegRow;           // forward declaration

namespace app
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
        static TransactionRow toRow(const finance::Transaction& transaction);

        [[nodiscard]]
        static finance::Transaction fromRow(const TransactionRow& row);

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
        static orm::WhereExpr toWhereExpr(
            const finance::TransactionFilter& filter
        );
    };

}   // namespace app

#endif   // __APP__SRC__APP__FACTORIES__TRANSACTION_FACTORY_HPP__
