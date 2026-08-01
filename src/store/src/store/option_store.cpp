#include "option_store.hpp"

#include <algorithm>

#include "config/id_types.hpp"
#include "exceptions/not_yet_implemented.hpp"
#include "finance/instrument/instrument_predicates.hpp"
#include "finance/predicates/predicates.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("Store.OptionStore");

namespace store
{
    /**
     * @brief Construct a new Option Store:: Option Store object
     *
     * @param instrumentService
     * @param instrumentIdSeq
     */
    OptionStore::OptionStore(
        InstrumentServicePtr instrumentService,
        InstrumentIdSeq&     instrumentIdSeq
    )
        : _instrumentService(std::move(instrumentService)),
          _instrumentIdSeq(instrumentIdSeq)
    {
        const auto options = _instrumentService->getOptions();

        _addCleanEntries(options.getValues());
    }

    /**
     * @brief Add an option to the store, this method checks if the option
     * already exists in the database to prevent duplicates, and if it does not
     * exist, it assigns a new instrument ID to the option, adds it to the
     * store, and returns the new instrument ID.
     *
     * @param option
     * @return std::expected<InstrumentId, OptionStoreResult>
     */
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

    /**
     * @brief Get the options for the given instrument IDs, this method
     * retrieves the options from the store that match the provided instrument
     * IDs, allowing callers to access specific options based on their
     * associated instrument IDs.
     *
     * @param instrumentIds The set of instrument IDs for which to retrieve
     * options.
     * @return finance::Options The options corresponding to the provided
     * instrument IDs.
     */
    finance::Options OptionStore::getOptions(
        const IdSet<InstrumentId>& instrumentIds
    ) const
    {
        const auto options = Options{
            .filter = finance::HasInstrumentIds<finance::Option>(instrumentIds),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        finance::Options result{_getValues(options)};

        if (!isFullCache())
        {
            const auto dbOptions =
                _instrumentService->getOptions(instrumentIds);

            for (const auto& option : dbOptions.getValues())
            {
                if (!result.contains(option.getId()))
                    result.addUnchecked(option);
            }
        }

        return result;
    }

    std::optional<finance::Option> OptionStore::getOption(
        InstrumentId instrumentId
    ) const
    {
        const auto& options = getOptions({instrumentId});

        if (options.empty())
            return std::nullopt;

        if (options.size() > 1)
        {
            LOG_ERROR(
                std::format(
                    "Multiple options found for instrument ID {}",
                    instrumentId.toString()
                )
            );
            // here we throw an exception because this should never happen, as
            // the instrument ID should be unique for each option, and if we
            // find multiple options with the same instrument ID
            throw std::runtime_error(
                "Multiple options found for instrument ID " +
                instrumentId.toString()
            );
        }

        return options.getValues().front();
    }

    /**
     * @brief Commit any pending changes in the store to the underlying service,
     * this method iterates through all entries in the store and performs the
     * necessary operations (add, update, delete) based on the state of each
     * entry, and then notifies listeners of the commit and updates the session
     * with the current values.
     *
     * @param reMap The ID remapping map to use for updating entries that
     * reference remapped IDs.
     */
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

                    if (!insertionResult)
                    {
                        throw std::runtime_error(
                            std::format(
                                "Failed to add option '{}' to database: {}",
                                entry.value.getName(),
                                insertionResult.error().toString()
                            )
                        );
                    }

                    const auto& insertion = insertionResult.value();

                    LOG_DEBUG(
                        std::format(
                            "Added new option: {} with ID: {} and "
                            "Instrument "
                            "ID: {}",
                            entry.value.toString(),
                            insertion.optionId.toString(),
                            insertion.instrumentId.toString()
                        )
                    );

                    const auto oldInstrumentId = entry.value.getInstrumentId();

                    auto option = entry.value;
                    option.setId(insertion.optionId);
                    option.setInstrumentId(insertion.instrumentId);

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

                    if (oldInstrumentId != insertion.instrumentId)
                        _instrumentIdMap[oldInstrumentId] =
                            insertion.instrumentId;

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

    /**
     * @brief Check if an option with the given details already exists in the
     * database, this is used to prevent duplicate entries and ensure data
     * integrity.
     *
     * @param option The Option object containing the details of the option to
     * check for existence
     * @return true if an option with the given details exists, false otherwise
     */
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

    /**
     * @brief Get the mapping of instrument IDs, this allows other parts of the
     * application to access the mapping of old instrument IDs to new instrument
     * IDs that have been remapped during commits, which is important for
     * maintaining consistency and ensuring that references to instruments are
     * updated correctly throughout the application.
     *
     * @return const IdIdMap<InstrumentId>& The mapping of old instrument IDs to
     * new instrument IDs.
     */
    const IdIdMap<InstrumentId>& OptionStore::getInstrumentIdMap() const
    {
        return _instrumentIdMap;
    }

    /**
     * @brief Remap instrument IDs in the store based on the given ID remapping
     * map, this is used to update entries in the store that reference
     * instrument IDs that have been remapped, ensuring that all references to
     * instruments are consistent with the new IDs after a commit.
     *
     * @param reMap
     */
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

    void OptionStore::reload()
    {
        LOG_ENTRY;

        _logCache(LOG_CATEGORY, LogLevel::Debug);

        _clearEntries();
        const auto options = _instrumentService->getOptions();
        _addCleanEntries(options.getValues());
    }

}   // namespace store
