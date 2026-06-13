#include "option_store.hpp"

#include <algorithm>

#include "config/id_types.hpp"
#include "exceptions/not_yet_implemented.hpp"
#include "finance/instrument/instrument_predicates.hpp"

namespace store
{
    OptionStore::OptionStore(
        InstrumentServicePtr instrumentService,
        InstrumentIdSeq&     instrumentIdSeq
    )
        : BaseStore<finance::Option, OptionId>(true),
          _instrumentService(std::move(instrumentService)),
          _instrumentIdSeq(instrumentIdSeq)
    {
        const auto options = _instrumentService->getOptions();

        _addCleanEntries(options);
    }

    std::expected<InstrumentId, OptionStoreResult> OptionStore::addOption(
        finance::Option option
    )
    {
        const auto name = option.getName();

        // check if the option exists in the db but not in store
        if (optionExists(option))
        {
            LOG_ERROR(
                std::format(
                    "Option with name {} already exists in the database",
                    name
                )
            );

            return std::unexpected(OptionStoreResult::OptionAlreadyExists);
        }

        const auto instrumentId = _instrumentIdSeq.next();
        option.setInstrumentId(instrumentId);
        _addEntry(std::move(option));

        return instrumentId;
    }

    void OptionStore::commit(const IdIdMap<InstrumentId>& reMap)
    {
        LOG_ENTRY;

        // make an early return to not notify unnecessarily
        if (!isDirty())
            return;

        _onInstrumentIdRemap(reMap);

        _instrumentIdMap.clear();

        for (const auto& entry : _getEntries())
        {
            switch (entry.state)
            {
                case StoreState::New:
                {
                    const auto insertionResult =
                        _instrumentService->addOption(entry.value);

                    LOG_DEBUG(
                        std::format(
                            "Added new option: {} with ID: {} and "
                            "Instrument "
                            "ID: {}",
                            entry.value.toString(),
                            insertionResult.optionId.toString(),
                            insertionResult.instrumentId.toString()
                        )
                    );

                    const auto oldInstrumentId = entry.value.getInstrumentId();

                    auto option = entry.value;
                    option.setId(insertionResult.optionId);
                    option.setInstrumentId(insertionResult.instrumentId);

                    const auto result = _commitEntry(
                        entry.value.getId(),
                        Entry{.value = option, .state = entry.state}
                    );

                    if (result != StoreResult::Ok)
                    {
                        throw std::runtime_error(
                            "Failed to add new option entry to database"
                        );
                    }

                    if (oldInstrumentId != insertionResult.instrumentId)
                        _instrumentIdMap[oldInstrumentId] =
                            insertionResult.instrumentId;

                    break;
                }
                case StoreState::Modified:
                case StoreState::Deleted:
                {
                    throw NotYetImplementedException(
                        StoreStateMeta::toString(entry.state) +
                        " not yet implemented"
                    );
                }
                case StoreState::Clean:
                {
                    break;
                }
            }
        }

        _notifyOnCommit();
    }

    bool OptionStore::optionExists(const finance::Option& option) const
    {
        const auto options = Options{
            .filter   = finance::HasOptionName(option.getName()),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        auto exists = _getEntry(options).has_value();

        if (!isFullCache())
        {
            exists |= _instrumentService->optionExists(option);
        }

        return exists;
    }

    const IdIdMap<InstrumentId>& OptionStore::getInstrumentIdMap() const
    {
        return _instrumentIdMap;
    }

    void OptionStore::_onInstrumentIdRemap(const IdIdMap<InstrumentId>& reMap)
    {
        LOG_ENTRY;

        for (const auto& entry : _getEntries())
        {
            if (entry.state != StoreState::New)
            {
                // check if this committed transaction references the
                // remapped ID
                const auto hasId = std::ranges::any_of(
                    reMap,
                    [&entry](const auto& pair)
                    { return entry.value.hasUnderlying(pair.first); }
                );

                if (hasId)
                {
                    throw std::runtime_error(
                        "Instrument ID found in already committed "
                        "transaction "
                        "entry!"
                    );
                }

                continue;
            }

            for (const auto& pair : reMap)
            {
                if (entry.value.hasUnderlying(pair.first))
                {
                    auto option = entry.value;
                    option.updateUnderlying(pair.second);
                    _updateEntry(option, StoreState::New);
                    break;
                }
            }
        }
    }

}   // namespace store