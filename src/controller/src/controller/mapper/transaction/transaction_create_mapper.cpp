#include "transaction_create_mapper.hpp"

#include "drafts/transaction/transaction_create_draft.hpp"
#include "finance/transaction/option_transaction.hpp"
#include "utils/finance.hpp"

namespace controller
{
    /**
     * @brief Maps a CreateCashTransactionDraft to a CashTransaction.
     *
     * @param draft
     * @return CashTransaction
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
            AccountId::invalid(),
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
            AccountId::invalid(),
            draft.getQuantity(),
            draft.getUnitPrice(),
            draft.getFees(),
            draft.getPositionId(),
            draft.getComment()
        };
    }

    /**
     * @brief Maps a CreateOptionTransactionDraft to an OptionTransaction.
     *
     * @param draft
     * @return finance::OptionTransaction
     */
    finance::OptionTransaction TransactionCreateMapper::fromCreateOptionDraft(
        const drafts::CreateOptionTransactionDraft& draft
    )
    {
        return finance::OptionTransaction{
            TransactionId::invalid(),   // will be created
            draft.getTimestamp(),
            TransactionStatus::Completed,
            draft.getInstrumentId(),
            draft.getUnderlyingInstrumentId(),
            draft.getSecurityAccount(),
            draft.getCashAccount(),
            AccountId::invalid(),
            draft.getQuantity(),
            draft.getStrikePrice(),
            draft.getAmount(),
            draft.getFees(),
            draft.getContractSize(),
            draft.getPositionId(),
            TransactionOptionAction::Open,
            draft.getBuySell(),
            draft.getOptionType(),
            std::nullopt,   // rolled option will be set when rolling
            draft.getComment()
        };
    }
}   // namespace controller
