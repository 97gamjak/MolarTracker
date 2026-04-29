#ifndef __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_MAPPER_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_MAPPER_HPP__

#include <variant>

namespace finance
{
    class Transaction;   // forward declaration
    struct CashData;     // forward declaration
    class TradeData;     // forward declaration

    using TransactionData =
        std::variant<CashData, TradeData>;   // forward declaration

}   // namespace finance

namespace drafts
{
    struct CreateCashTransactionDraft;

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
        // static TransactionDraft toDraft(
        //     const finance::Transaction& transaction
        // );

        static finance::Transaction fromCreateCashTransactionDraft(
            const CreateCashTransactionDraft& draft
        );
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_MAPPER_HPP__