#ifndef __REPO__SRC__REPO__FACTORIES__TRANSACTION_FACTORY_HPP__
#define __REPO__SRC__REPO__FACTORIES__TRANSACTION_FACTORY_HPP__

#include <optional>

#include "config/id_types.hpp"
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
        static finance::DomainTransaction fromRow(
            const TransactionRow&                      row,
            const std::optional<TransactionOptionRow>& optionRow
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
    };

}   // namespace repo

#endif   // __REPO__SRC__REPO__FACTORIES__TRANSACTION_FACTORY_HPP__
