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

    drafts::TransactionEntryDraft TransactionMapper::toEntryDraft(
        const finance::TransactionEntry& entry
    )
    {
        return TransactionEntryDraft{entry.getAccountId(), entry.getCash()};
    }

    finance::TransactionEntry TransactionMapper::fromEntryDraft(
        const drafts::TransactionEntryDraft& entryDraft
    )
    {
        return finance::TransactionEntry{
            TransactionEntryId::invalid(),
            entryDraft.getAccountId(),
            entryDraft.getCash()
        };
    }

    finance::Transaction TransactionMapper::fromCreateCashTransactionDraft(
        const CreateCashTransactionDraft& draft
    )
    {
        std::vector<finance::TransactionEntry> entries;

        for (const auto& entryDraft : draft.getEntries())
            entries.push_back(fromEntryDraft(entryDraft));

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

    std::vector<drafts::TransactionOverviewDraft> TransactionMapper::
        toOverviewDrafts(const std::vector<finance::Transaction>& transactions)
    {
        std::vector<drafts::TransactionOverviewDraft> drafts;
        drafts.reserve(transactions.size());

        for (const auto& transaction : transactions)
            drafts.emplace_back(toOverviewDraft(transaction));

        return drafts;
    }

    drafts::TransactionOverviewDraft TransactionMapper::toOverviewDraft(
        const finance::Transaction& transaction
    )
    {
        std::vector<drafts::TransactionEntryDraft> entryDrafts;
        entryDrafts.reserve(transaction.getEntries().size());

        for (const auto& entry : transaction.getEntries())
        {
            entryDrafts.push_back(toEntryDraft(entry));
        }

        return drafts::TransactionOverviewDraft{
            transaction.getTimestamp(),
            std::move(entryDrafts),
            transaction.getComment()
        };
    }

}   // namespace drafts