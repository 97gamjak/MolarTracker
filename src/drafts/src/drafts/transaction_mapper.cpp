#include "drafts/transaction_mapper.hpp"

#include <string>
#include <unordered_map>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "drafts/transaction_draft.hpp"
#include "finance/trade_data.hpp"
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

    finance::TradeLeg TransactionMapper::fromTradeLegDraft(
        const TradeLegDraft& draft
    )
    {
        return finance::TradeLeg{
            draft.getAccountId(),
            draft.getInstrumentId(),
            draft.getQuantity(),
            draft.getUnitPrice(),
        };
    }

    std::vector<finance::TradeLeg> TransactionMapper::fromTradeLegDrafts(
        const std::vector<TradeLegDraft>& drafts
    )
    {
        std::vector<finance::TradeLeg> legs;
        legs.reserve(drafts.size());

        for (const auto& draft : drafts)
            legs.push_back(fromTradeLegDraft(draft));

        return legs;
    }

    TradeLegDraft TransactionMapper::toTradeLegDraft(
        const finance::TradeLeg& leg,
        const std::unordered_map<
            InstrumentId,
            std::string,
            typename InstrumentId::Hash>& instrumentNames
    )
    {
        return TradeLegDraft{
            leg.getAccountId(),
            leg.getUnitPrice(),
            leg.getQuantity(),
            instrumentNames.at(leg.getInstrumentId())
        };
    }

    std::vector<TradeLegDraft> TransactionMapper::toTradeLegDrafts(
        const std::vector<finance::TradeLeg>& legs,
        const std::unordered_map<
            InstrumentId,
            std::string,
            typename InstrumentId::Hash>& instrumentNames
    )
    {
        std::vector<TradeLegDraft> drafts;
        drafts.reserve(legs.size());

        for (const auto& leg : legs)
        {
            drafts.push_back(toTradeLegDraft(leg, instrumentNames));
        }

        return drafts;
    }

    finance::Transaction TransactionMapper::fromCreateStockTransactionDraft(
        const CreateStockTransactionDraft& draft
    )
    {
        std::vector<finance::TransactionEntry> entries;

        for (const auto& entryDraft : draft.getEntries())
            entries.push_back(fromEntryDraft(entryDraft));

        finance::Transaction transaction{
            TransactionId::invalid(),
            draft.getTimestamp(),
            TransactionStatus::Completed,
            finance::TradeData{fromTradeLegDrafts(draft.getLegs())},
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
        toOverviewDrafts(
            const std::vector<finance::Transaction>& transactions,
            const std::unordered_map<
                InstrumentId,
                std::string,
                typename InstrumentId::Hash>& instrumentNames
        )
    {
        std::vector<drafts::TransactionOverviewDraft> drafts;
        drafts.reserve(transactions.size());

        for (const auto& transaction : transactions)
            drafts.emplace_back(toOverviewDraft(transaction, instrumentNames));

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
        const finance::Transaction& transaction,
        const std::unordered_map<
            InstrumentId,
            std::string,
            typename InstrumentId::Hash>& instrumentNames
    )
    {
        std::vector<drafts::TransactionEntryDraft> entryDrafts;
        entryDrafts.reserve(transaction.getEntries().size());

        for (const auto& entry : transaction.getEntries())
            entryDrafts.push_back(toEntryDraft(entry));

        struct Visitor
        {
            const std::unordered_map<
                InstrumentId,
                std::string,
                typename InstrumentId::Hash>& instrumentNames;

            std::vector<TradeLegDraft> operator()(const finance::TradeData& data
            )
            {
                return TransactionMapper::toTradeLegDrafts(
                    data.getLegs(),
                    instrumentNames
                );
            }

            std::vector<TradeLegDraft> operator()(const finance::
                                                      CashData& /*data*/)
            {
                return {};
            }
        };

        return drafts::TransactionOverviewDraft{
            transaction.getType(),
            transaction.getTimestamp(),
            std::move(entryDrafts),
            std::visit(Visitor{instrumentNames}, transaction.getData()),
            transaction.getComment()
        };
    }

}   // namespace drafts