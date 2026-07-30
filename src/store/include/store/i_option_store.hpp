#ifndef __STORE__INCLUDE__STORE__I_OPTION_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_OPTION_STORE_HPP__

#include <cstdint>
#include <expected>

#include "common/container/id_id_map.hpp"
#include "common/container/set.hpp"
#include "config/id_types.hpp"
#include "finance/instrument/options.hpp"

namespace finance
{
    class Option;   // forward declaration
}   // namespace finance

namespace store
{
    enum class OptionStoreResult : std::uint8_t
    {
        Ok,
        Error,
        OptionAlreadyExists,
        OptionNotFound,
    };

    /**
     * @brief Store for managing options, this interface defines the operations
     * that can be performed on the option store, such as adding new options,
     * retrieving the mapping of instrument IDs, and committing changes to the
     * store, allowing for a structured way to manage options within the
     * application and interact with the underlying data storage for options.
     *
     */
    class IOptionStore
    {
       public:
        virtual ~IOptionStore() = default;

        /**
         * @brief Add a new option to the store, this will insert the option
         * into the store and return the generated instrument ID for the option
         * if the operation is successful, or an error result if the option
         * already exists or if there was an error during the operation.
         *
         * @param option The Option object containing the details of the option
         * to add to the store
         * @return std::expected<InstrumentId, OptionStoreResult> The generated
         * InstrumentId if successful, or an OptionStoreResult error code if
         * there was an error or if the option already exists in the store
         */
        [[nodiscard]]
        virtual std::expected<InstrumentId, OptionStoreResult> addOption(
            finance::Option option
        ) = 0;

        /**
         * @brief Get the mapping of instrument IDs to their remapped IDs, this
         * is used to track the remapping of instrument IDs during commit
         * operations, allowing callers to access the current state of ID
         * remapping in the store and understand how original instrument IDs
         * have been mapped to new IDs after changes have been committed.
         *
         * @return const IdIdMap<InstrumentId>& The mapping of original
         * InstrumentIds to their remapped InstrumentIds after commit operations
         */
        [[nodiscard]]
        virtual const IdIdMap<InstrumentId>& getInstrumentIdMap() const = 0;

        /**
         * @brief Get a list of all options in the store for the given
         * instrument IDs, this will return all options that are not marked as
         * deleted, and will include options that are new or modified but not
         * yet committed.
         *
         * @param instrumentIds The set of instrument IDs to retrieve options
         * for
         * @return finance::Options
         */
        [[nodiscard]]
        virtual finance::Options getOptions(
            const IdSet<InstrumentId>& instrumentIds
        ) const = 0;

        /**
         * @brief Get an option by its instrument ID, this allows callers to
         * retrieve a specific option from the store based on its instrument ID,
         * enabling access to the details of the option for further processing
         * or analysis.
         *
         * @param instrumentId The instrument ID of the option to retrieve
         * @return std::optional<finance::Option> The Option object if found, or
         * an empty optional if no option with the given instrument ID exists in
         * the store
         */
        [[nodiscard]]
        virtual std::optional<finance::Option> getOption(
            InstrumentId instrumentId
        ) const = 0;
    };
}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_OPTION_STORE_HPP__