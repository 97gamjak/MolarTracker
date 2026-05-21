#include "drafts/transaction_mapper.hpp"

#include <string>

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
     * TransactionEntryDraft
     *
     * @param entry
     * @param externalAccounts
     *
     * @return TransactionEntryDraft
     */
    TransactionEntryDraft TransactionMapper::toEntryDraft(
        const finance::TransactionEntry& entry,
        const idSet<AccountId>&          externalAccounts
    )
    {
        return TransactionEntryDraft{
            entry.getAccountId(),
            entry.getCash(),
            entry.getType(),
            externalAccounts.contains(entry.getAccountId())
        };
    }

    /**
     * @brief Converts a TransactionEntryDraft to a
     * finance::TransactionEntry
     *
     * @param entryDraft
     * @return finance::TransactionEntry
     */
    finance::TransactionEntry TransactionMapper::fromEntryDraft(
        const TransactionEntryDraft& entryDraft
    )
    {
        return finance::TransactionEntry{
            TransactionEntryId::invalid(),
            entryDraft.getAccountId(),
            entryDraft.getCash(),
            entryDraft.getType()
        };
    }

    /**
     * @brief Converts a CreateCashTransactionDraft to a
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
     * @brief Converts a TradeLegDraft to a finance::TradeLeg
     *
     * @param draft
     * @return finance::TradeLeg
     */
    finance::TradeLeg TransactionMapper::fromTradeLegDraft(
        const TradeLegDraft& draft
    )
    {
        return finance::TradeLeg{
            draft.getAccountId(),
            draft.getInstrumentId(),
            draft.getQuantity(),
            draft.getUnitPrice(),
            draft.getPositionId()
        };
    }

    /**
     * @brief Converts a vector of TradeLegDraft to a vector of
     * finance::TradeLeg
     *
     * @param drafts
     * @return std::vector<finance::TradeLeg>
     */
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

    /**
     * @brief Converts a finance::TradeLeg to a TradeLegDraft
     *
     * @param leg
     * @param instrumentNames
     * @return TradeLegDraft
     */
    TradeLegDraft TransactionMapper::toTradeLegDraft(
        const finance::TradeLeg&          leg,
        const instrumentMap<std::string>& instrumentNames
    )
    {
        return TradeLegDraft{
            leg.getAccountId(),
            leg.getUnitPrice(),
            leg.getQuantity(),
            instrumentNames.at(leg.getInstrumentId())
        };
    }

    /**
     * @brief Converts a vector of finance::TradeLeg to a vector of
     * TradeLegDraft
     *
     * @param legs
     * @param instrumentNames
     * @return std::vector<TradeLegDraft>
     */
    std::vector<TradeLegDraft> TransactionMapper::toTradeLegDrafts(
        const std::vector<finance::TradeLeg>& legs,
        const instrumentMap<std::string>&     instrumentNames
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

    /**
     * @brief Converts a CreateStockTransactionDraft to a
     * finance::Transaction
     *
     * @param draft
     * @return finance::Transaction
     */
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
     * TransactionOverviewDraft
     *
     * @param transactions
     * @param instrumentNames
     * @param externalAccounts
     *
     * @return std::vector<TransactionOverviewDraft>
     */
    std::vector<TransactionOverviewDraft> TransactionMapper::toOverviewDrafts(
        const std::vector<finance::Transaction>& transactions,
        const instrumentMap<std::string>&        instrumentNames,
        const idSet<AccountId>&                  externalAccounts
    )
    {
        std::vector<TransactionOverviewDraft> drafts;
        drafts.reserve(transactions.size());

        for (const auto& transaction : transactions)
            drafts.emplace_back(
                toOverviewDraft(transaction, instrumentNames, externalAccounts)
            );

        return drafts;
    }

    /**
     * @brief Converts a finance::Transaction to a
     * TransactionOverviewDraft
     *
     * @param transaction
     * @param instrumentNames
     * @param externalAccounts
     *
     * @return TransactionOverviewDraft
     */
    TransactionOverviewDraft TransactionMapper::toOverviewDraft(
        const finance::Transaction&       transaction,
        const instrumentMap<std::string>& instrumentNames,
        const idSet<AccountId>&           externalAccounts
    )
    {
        std::vector<TransactionEntryDraft> entryDrafts;
        entryDrafts.reserve(transaction.getEntries().size());

        for (const auto& entry : transaction.getEntries())
            entryDrafts.push_back(toEntryDraft(entry, externalAccounts));

        struct Visitor
        {
            const instrumentMap<std::string>& instrumentNames;

            ~Visitor() = default;

            explicit Visitor(const instrumentMap<std::string>& instrumentNames_)
                : instrumentNames(instrumentNames_)
            {
            }
            // default copy and move constructors
            Visitor(const Visitor&)            = delete;
            Visitor& operator=(const Visitor&) = delete;
            Visitor(Visitor&&)                 = delete;
            Visitor& operator=(Visitor&&)      = delete;

            std::vector<TradeLegDraft> operator()(const finance::TradeData& data
            ) const
            {
                return TransactionMapper::toTradeLegDrafts(
                    data.getLegs(),
                    instrumentNames
                );
            }

            std::vector<TradeLegDraft> operator()(const finance::
                                                      CashData& /*data*/) const
            {
                return {};
            }
        };

        return TransactionOverviewDraft{
            transaction.getType(),
            transaction.getTimestamp(),
            std::move(entryDrafts),
            std::visit(Visitor{instrumentNames}, transaction.getData()),
            transaction.getComment()
        };
    }

}   // namespace drafts