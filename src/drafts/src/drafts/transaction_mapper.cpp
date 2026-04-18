#include "drafts/transaction_mapper.hpp"

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "drafts/transaction_draft.hpp"
#include "finance/cash.hpp"
#include "finance/transaction.hpp"

namespace drafts
{

    struct ToDraftVisitor
    {
        TransactionEntryDraft operator()(const finance::CashTransaction& detail
        ) const
        {
            return TransactionEntryDraft{
                AccountId::invalid(),
                detail.getAmount(),
                detail.getCurrency(),
                AccountKind::Cash
            };
        }
    };

    TransactionDraft TransactionMapper::toDraft(
        const finance::Transaction& transaction
    )
    {
        TransactionDraft draft;

        draft.timestamp = transaction.getTimestamp();
        draft.comment   = transaction.getComment();
        draft.id        = transaction.getId();

        for (const auto& entry : transaction.getEntries())
        {
            auto entryDraft = std::visit(ToDraftVisitor{}, entry.getDetails());
            entryDraft.accountId = entry.getAccountId();

            draft.entries.push_back(entryDraft);
        }

        return draft;
    }

    finance::Transaction TransactionMapper::toTransaction(
        const TransactionDraft& draft,
        TransactionId           id
    )
    {
        std::vector<finance::TransactionEntry> entries;

        finance::Transaction transaction{
            id,
            draft.timestamp,
            TransactionStatus::Completed,
            draft.comment,
        };

        for (const auto& entryDraft : draft.entries)
        {
            switch (entryDraft.accountKind)
            {
                case AccountKind::Cash:
                {
                    const auto cash =
                        finance::Cash{entryDraft.currency, entryDraft.amount};

                    const auto entry = finance::TransactionEntry{
                        TransactionEntryId::invalid(),
                        entryDraft.accountId,
                        finance::CashTransaction{cash}
                    };

                    transaction.addEntry(entry);

                    break;
                }
                case AccountKind::External:
                    break;
            }
        }

        return transaction;
    }

}   // namespace drafts