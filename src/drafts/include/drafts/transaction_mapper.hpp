#ifndef __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_MAPPER_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_MAPPER_HPP__

#include <variant>
#include <vector>

namespace finance
{
    struct CashData;          // forward declaration
    class TradeData;          // forward declaration
    class TransactionEntry;   // forward declaration
    class Transaction;        // forward declaration

    using TransactionData =
        std::variant<CashData, TradeData>;   // forward declaration

}   // namespace finance

namespace drafts
{
    class CreateCashTransactionDraft;   // forward declaration
    class TransactionOverviewDraft;     // forward declaration
    class TransactionEntryDraft;        // forward declaration

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
        static drafts::TransactionEntryDraft toEntryDraft(
            const finance::TransactionEntry& entry
        );

        static finance::TransactionEntry fromEntryDraft(
            const drafts::TransactionEntryDraft& entryDraft
        );

        static finance::Transaction fromCreateCashTransactionDraft(
            const CreateCashTransactionDraft& draft
        );

        static std::vector<drafts::TransactionOverviewDraft> toOverviewDrafts(
            const std::vector<finance::Transaction>& transactions
        );

        static drafts::TransactionOverviewDraft toOverviewDraft(
            const finance::Transaction& transaction
        );
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_MAPPER_HPP__