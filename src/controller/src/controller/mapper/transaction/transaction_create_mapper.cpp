#include "transaction_create_mapper.hpp"

#include "config/finance.hpp"
#include "drafts/transaction/transaction_create_draft.hpp"

namespace controller
{
    /**
     * @brief Maps a CreateCashTransactionDraft to a CashTransaction.
     *
     * @param draft
     * @return finance::CashTransaction
     */
    finance::CashTransaction TransactionCreateMapper::fromCreateCashDraft(
        const drafts::CreateCashTransactionDraft& draft
    )
    {
        return finance::CashTransaction{
            TransactionId::invalid(),   // will be created
            draft.getTimestamp(),
            TransactionStatus::Completed,
            draft.getAccountId(),
            draft.getExternalAccount(),
            draft.getAmount(),
            draft.getFees(),
            draft.getComment()
        };
    }

    /**
     * @brief Maps a CreateStockTransactionDraft to a StockTransaction.
     *
     * @param draft
     * @return finance::StockTransaction
     */
    finance::StockTransaction TransactionCreateMapper::fromCreateStockDraft(
        const drafts::CreateStockTransactionDraft& draft
    )
    {
        return finance::StockTransaction{
            TransactionId::invalid(),   // will be created
            draft.getTimestamp(),
            TransactionStatus::Completed,
            draft.getInstrumentId(),
            draft.getSecurityAccount(),
            draft.getCashAccount(),
            draft.getExternalAccount(),
            draft.getQuantity(),
            draft.getUnitPrice(),
            draft.getFees(),
            draft.getPositionId(),
            draft.getComment()
        };
    }
}   // namespace controller
