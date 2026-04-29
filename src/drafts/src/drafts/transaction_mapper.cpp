#include "drafts/transaction_mapper.hpp"

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "drafts/transaction_draft.hpp"
#include "finance/cash.hpp"
#include "finance/transaction.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("Drafts.TransactionMapper");

namespace drafts
{

    // TransactionDraft TransactionMapper::toDraft(
    //     const finance::Transaction& transaction
    // )
    // {
    //     TransactionDraft draft;

    //     draft.timestamp = transaction.getTimestamp();
    //     draft.comment   = transaction.getComment();
    //     draft.id        = transaction.getId();

    //     for (const auto& entry : transaction.getEntries())
    //     {
    //         TransactionEntryDraft entryDraft{
    //             AccountId::invalid(),
    //             entry.getCash()
    //         };

    //         entryDraft.accountId = entry.getAccountId();

    //         draft.entries.push_back(entryDraft);
    //     }

    //     return draft;
    // }

    finance::Transaction TransactionMapper::fromCreateCashTransactionDraft(
        const CreateCashTransactionDraft& draft
    )
    {
        std::vector<finance::TransactionEntry> entries;

        for (const auto& entryDraft : draft.getEntries())
        {
            const finance::TransactionEntry entry{
                TransactionEntryId::invalid(),
                entryDraft.accountId,
                entryDraft.cash
            };

            entries.push_back(entry);
        }

        finance::Transaction transaction{
            TransactionId::invalid(),
            draft.getTimestamp(),
            TransactionStatus::Completed,
            finance::CashData{},
            entries,
            draft.getComment()
        };

        return transaction;
    }

}   // namespace drafts