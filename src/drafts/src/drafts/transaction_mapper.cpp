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

    /**
     * @brief Visitor struct for converting TransactionEntryDetails to
     * TransactionEntryDraft
     *
     * This struct is used with std::visit to convert different types of
     * TransactionEntryDetails (e.g., CashTransaction) to a common
     * TransactionEntryDraft format. This allows us to handle different types of
     * transaction entries in a uniform way when creating drafts.
     */
    struct ToDraftVisitor
    {
        /**
         * @brief operator() overload for handling CashTransaction details, this
         * will convert a CashTransaction detail into a TransactionEntryDraft
         *
         * @param detail
         * @return TransactionEntryDraft
         */
        TransactionEntryDraft operator()(const finance::CashTransaction& detail
        ) const
        {
            return TransactionEntryDraft{
                AccountId::invalid(),
                detail.getCash(),
                AccountKind::Cash
            };
        }
    };

    /**
     * @brief Convert a finance::Transaction to a TransactionDraft
     *
     * @param transaction The finance::Transaction to convert, this is the
     * domain model representation of a transaction, and contains all the
     * details of the transaction as it exists in the business logic layer.
     * @return TransactionDraft The corresponding TransactionDraft, this is the
     * draft model representation of a transaction, and is used for transferring
     * data between the business logic and the UI, it contains the same
     * information as the finance::Transaction but may be structured differently
     * to better suit the needs of the UI.
     */
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

    /**
     * @brief Convert a TransactionDraft to a finance::Transaction
     *
     * @param draft The TransactionDraft to convert, this is the draft model
     * representation of a transaction, and is used for transferring data
     * between the business logic and the UI, it contains the same information
     * as the finance::Transaction but may be structured differently to better
     * suit the needs of the UI.
     * @param id The ID to assign to the resulting finance::Transaction, this is
     * used to ensure that the transaction has a valid ID when it is created
     * from the draft, and allows us to maintain the association between the
     * draft and the domain model representation of the transaction.
     * @return finance::Transaction The corresponding finance::Transaction, this
     * is the domain model representation of a transaction, and contains all the
     * details of the transaction as it exists in the business logic layer.
     */
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
                    const auto entry = finance::TransactionEntry{
                        TransactionEntryId::invalid(),
                        entryDraft.accountId,
                        finance::CashTransaction{entryDraft.cash}
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