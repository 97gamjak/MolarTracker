#include "drafts/transaction_mapper.hpp"

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "drafts/transaction_draft.hpp"
#include "finance/transaction.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("Drafts.TransactionMapper");

namespace drafts
{

    /**
     * @brief Converts a finance::TransactionEntry to a
     * drafts::TransactionEntryDraft
     *
     * @param entry
     * @return drafts::TransactionEntryDraft
     */
    drafts::TransactionEntryDraft TransactionMapper::toEntryDraft(
        const finance::TransactionEntry& entry
    )
    {
        return TransactionEntryDraft{entry.getAccountId(), entry.getCash()};
    }

    /**
     * @brief Converts a drafts::TransactionEntryDraft to a
     * finance::TransactionEntry
     *
     * @param entryDraft
     * @return finance::TransactionEntry
     */
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

    /**
     * @brief Converts a drafts::CreateCashTransactionDraft to a
     * finance::Transaction
     *
     * @param draft
     * @return finance::Transaction
     */
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

    /**
     * @brief Converts a vector of finance::Transaction to a vector of
     * drafts::TransactionOverviewDraft
     *
     * @param transactions
     * @return std::vector<drafts::TransactionOverviewDraft>
     */
    std::vector<drafts::TransactionOverviewDraft> TransactionMapper::
        toOverviewDrafts(const std::vector<finance::Transaction>& transactions)
    {
        std::vector<drafts::TransactionOverviewDraft> drafts;
        drafts.reserve(transactions.size());

        for (const auto& transaction : transactions)
            drafts.emplace_back(toOverviewDraft(transaction));

        return drafts;
    }

    /**
     * @brief Converts a finance::Transaction to a
     * drafts::TransactionOverviewDraft
     *
     * @param transaction
     * @return drafts::TransactionOverviewDraft
     */
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