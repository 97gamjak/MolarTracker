#include "transaction_mapper.hpp"

#include <string>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "drafts/transaction_draft.hpp"
#include "finance/transaction/trade_data.hpp"
#include "finance/transaction/transaction.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("Controller.Mapper.TransactionMapper");

namespace controller
{

    /**
     * @brief Converts a finance::TransactionEntry to a
     * drafts::TransactionEntryDraft
     *
     * @param entry
     * @param externalAccounts
     *
     * @return drafts::TransactionEntryDraft
     */
    drafts::TransactionEntryDraft TransactionMapper::toEntryDraft(
        const finance::TransactionEntry& entry,
        const idSet<AccountId>&          externalAccounts
    )
    {
        return drafts::TransactionEntryDraft{
            entry.getAccountId(),
            entry.getCash(),
            entry.getType(),
            externalAccounts.contains(entry.getAccountId())
        };
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
            entryDraft.getCash(),
            entryDraft.getType()
        };
    }

    /**
     * @brief Converts a drafts::CreateCashTransactionDraft to a
     * finance::DomainTransaction
     *
     * @param draft
     * @return finance::DomainTransaction
     */
    finance::DomainTransaction TransactionMapper::
        fromCreateCashTransactionDraft(
            const drafts::CreateCashTransactionDraft& draft
        )
    {
        std::vector<finance::TransactionEntry> entries;

        for (const auto& entryDraft : draft.getEntries())
            entries.push_back(fromEntryDraft(entryDraft));

        finance::DomainTransaction transaction{
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
     * @brief Converts a drafts::TradeLegDraft to a finance::TradeLeg
     *
     * @param draft
     * @return finance::TradeLeg
     */
    finance::TradeLeg TransactionMapper::fromTradeLegDraft(
        const drafts::TradeLegDraft& draft
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
     * @brief Converts a vector of drafts::TradeLegDraft to a vector of
     * finance::TradeLeg
     *
     * @param drafts
     * @return std::vector<finance::TradeLeg>
     */
    std::vector<finance::TradeLeg> TransactionMapper::fromTradeLegDrafts(
        const std::vector<drafts::TradeLegDraft>& drafts
    )
    {
        std::vector<finance::TradeLeg> legs;
        legs.reserve(drafts.size());

        for (const auto& draft : drafts)
            legs.push_back(fromTradeLegDraft(draft));

        return legs;
    }

    /**
     * @brief Converts a finance::TradeLeg to a drafts::TradeLegDraft
     *
     * @param leg
     * @param instrumentNames
     * @return drafts::TradeLegDraft
     */
    drafts::TradeLegDraft TransactionMapper::toTradeLegDraft(
        const finance::TradeLeg&          leg,
        const instrumentMap<std::string>& instrumentNames
    )
    {
        std::string instrumentName = "UNKNOWN";

        if (instrumentNames.contains(leg.getInstrumentId()))
        {
            instrumentName = instrumentNames.at(leg.getInstrumentId());
        }
        else
        {
            LOG_WARNING(
                "Unknown instrument ID: " + leg.getInstrumentId().toString()
            );
        }

        return drafts::TradeLegDraft{
            leg.getAccountId(),
            leg.getUnitPrice(),
            leg.getQuantity(),
            instrumentName
        };
    }

    /**
     * @brief Converts a vector of finance::TradeLeg to a vector of
     * drafts::TradeLegDraft
     *
     * @param legs
     * @param instrumentNames
     * @return std::vector<drafts::TradeLegDraft>
     */
    std::vector<drafts::TradeLegDraft> TransactionMapper::toTradeLegDrafts(
        const std::vector<finance::TradeLeg>& legs,
        const instrumentMap<std::string>&     instrumentNames
    )
    {
        std::vector<drafts::TradeLegDraft> drafts;
        drafts.reserve(legs.size());

        for (const auto& leg : legs)
        {
            drafts.push_back(toTradeLegDraft(leg, instrumentNames));
        }

        return drafts;
    }

    /**
     * @brief Converts a drafts::CreateStockTransactionDraft to a
     * finance::DomainTransaction
     *
     * @param draft
     * @return finance::DomainTransaction
     */
    finance::DomainTransaction TransactionMapper::
        fromCreateStockTransactionDraft(
            const drafts::CreateStockTransactionDraft& draft
        )
    {
        std::vector<finance::TransactionEntry> entries;

        for (const auto& entryDraft : draft.getEntries())
            entries.push_back(fromEntryDraft(entryDraft));

        finance::DomainTransaction transaction{
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
     * @brief Converts a vector of finance::DomainTransaction to a vector of
     * drafts::TransactionOverviewDraft
     *
     * @param transactions
     * @param instrumentNames
     * @param externalAccounts
     *
     * @return std::vector<drafts::TransactionOverviewDraft>
     */
    std::vector<drafts::TransactionOverviewDraft> TransactionMapper::
        toOverviewDrafts(
            const std::vector<finance::DomainTransaction>& transactions,
            const instrumentMap<std::string>&              instrumentNames,
            const idSet<AccountId>&                        externalAccounts
        )
    {
        std::vector<drafts::TransactionOverviewDraft> drafts;
        drafts.reserve(transactions.size());

        for (const auto& transaction : transactions)
            drafts.emplace_back(
                toOverviewDraft(transaction, instrumentNames, externalAccounts)
            );

        return drafts;
    }

    /**
     * @brief Converts a finance::DomainTransaction to a
     * drafts::TransactionOverviewDraft
     *
     * @param transaction
     * @param instrumentNames
     * @param externalAccounts
     *
     * @return drafts::TransactionOverviewDraft
     */
    drafts::TransactionOverviewDraft TransactionMapper::toOverviewDraft(
        const finance::DomainTransaction& transaction,
        const instrumentMap<std::string>& instrumentNames,
        const idSet<AccountId>&           externalAccounts
    )
    {
        std::vector<drafts::TransactionEntryDraft> entryDrafts;
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

            std::vector<drafts::TradeLegDraft> operator()(
                const finance::TradeData& data
            ) const
            {
                return TransactionMapper::toTradeLegDrafts(
                    data.getLegs(),
                    instrumentNames
                );
            }

            std::vector<drafts::TradeLegDraft> operator()(
                const finance::CashData& /*data*/
            ) const
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

}   // namespace controller
