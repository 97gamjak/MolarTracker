#ifndef __CONTROLLER__SRC__CONTROLLER__MAPPER__TRANSACTION_MAPPER_HPP__

#define __CONTROLLER__SRC__CONTROLLER__MAPPER__TRANSACTION_MAPPER_HPP__

#include <string>
#include <variant>
#include <vector>

#include "config/id_types.hpp"

namespace finance
{
    struct CashData;          // forward declaration
    class TradeData;          // forward declaration
    class TransactionEntry;   // forward declaration
    class Transaction;        // forward declaration
    class TradeLeg;           // forward declaration

    using TransactionData =
        std::variant<CashData, TradeData>;   // forward declaration

}   // namespace finance

namespace drafts
{
    class CreateCashTransactionDraft;    // forward declaration
    class TransactionOverviewDraft;      // forward declaration
    class TransactionEntryDraft;         // forward declaration
    class TradeLegDraft;                 // forward declaration
    class CreateStockTransactionDraft;   // forward declaration
}   // namespace drafts

namespace controller
{

    /**
     * @brief Mapper class for converting between Transaction and
     * TransactionDraft
     *
     * This class provides static methods to convert between the domain model
     * (finance::Transaction) and the draft model (drafts::TransactionDraft).
     * This is useful for separating the concerns of the business logic and the
     * UI, and allows us to have a clear mapping between the two representations
     * of a transaction.
     */
    class TransactionMapper
    {
       public:
        [[nodiscard]]
        static drafts::TransactionEntryDraft toEntryDraft(
            const finance::TransactionEntry& entry,
            const idSet<AccountId>&          externalAccounts
        );

        [[nodiscard]]
        static finance::TransactionEntry fromEntryDraft(
            const drafts::TransactionEntryDraft& entryDraft
        );

        [[nodiscard]]
        static finance::Transaction fromCreateCashTransactionDraft(
            const drafts::CreateCashTransactionDraft& draft
        );

        [[nodiscard]]
        static finance::Transaction fromCreateStockTransactionDraft(
            const drafts::CreateStockTransactionDraft& draft
        );

        [[nodiscard]]
        static finance::TradeLeg fromTradeLegDraft(
            const drafts::TradeLegDraft& draft
        );

        [[nodiscard]]
        static std::vector<finance::TradeLeg> fromTradeLegDrafts(
            const std::vector<drafts::TradeLegDraft>& drafts
        );

        [[nodiscard]]
        static drafts::TradeLegDraft toTradeLegDraft(
            const finance::TradeLeg&          leg,
            const instrumentMap<std::string>& instrumentNames
        );

        [[nodiscard]]
        static std::vector<drafts::TradeLegDraft> toTradeLegDrafts(
            const std::vector<finance::TradeLeg>& legs,
            const instrumentMap<std::string>&     instrumentNames
        );

        [[nodiscard]]
        static std::vector<drafts::TransactionOverviewDraft> toOverviewDrafts(
            const std::vector<finance::Transaction>& transactions,
            const instrumentMap<std::string>&        instrumentNames,
            const idSet<AccountId>&                  externalAccounts
        );

        [[nodiscard]]
        static drafts::TransactionOverviewDraft toOverviewDraft(
            const finance::Transaction&       transaction,
            const instrumentMap<std::string>& instrumentNames,
            const idSet<AccountId>&           externalAccounts
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MAPPER__TRANSACTION_MAPPER_HPP__
