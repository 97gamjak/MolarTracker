#include "option_store.hpp"

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

    OptionStoreResult OptionStore::addOption(finance::Option option)
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

            return OptionStoreResult::OptionAlreadyExists;
        }

        option.setInstrumentId(_instrumentIdSeq.next());
        _addEntry(std::move(option));

        return OptionStoreResult::Ok;
    }

    void OptionStore::commit()
    {
        LOG_ENTRY;

        // make an early return to not notify unnecessarily
        if (!isDirty())
            return;

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

                    auto option = entry.value;
                    option.setId(insertionResult.optionId);
                    option.setInstrumentId(insertionResult.instrumentId);

                    const auto oldInstrumentId = entry.value.getInstrumentId();

                    const auto result = _commitEntry(
                        entry.value.getId(),
                        Entry{.value = option, .state = entry.state}
                    );

                    if (result != StoreResult::Ok)
                    {
                        throw std::runtime_error(
                            "Failed to add new stock entry to database"
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

}   // namespace store