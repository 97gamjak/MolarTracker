#include "transaction_mapper.hpp"

#include <string>

#include "config/id_types.hpp"
#include "drafts/transaction/transaction_draft.hpp"
#include "finance/transaction/trade_data.hpp"
#include "finance/transaction/transaction_entry.hpp"
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
        const finance::TradeLeg&                         leg,
        const unorderedIdMap<InstrumentId, std::string>& instrumentNames
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
        const std::vector<finance::TradeLeg>&            legs,
        const unorderedIdMap<InstrumentId, std::string>& instrumentNames
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

}   // namespace controller
