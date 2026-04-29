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
        const finance::Transaction& /*transaction*/
    )
    {
        // TODO: implement this properly, for now just return an empty draft
        return drafts::TransactionOverviewDraft{};
    }

}   // namespace drafts