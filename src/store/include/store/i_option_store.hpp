#ifndef __STORE__INCLUDE__STORE__I_OPTION_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_OPTION_STORE_HPP__

#include <cstdint>
#include <expected>

#include "config/id_types.hpp"
#include "utils/container/id_id_map.hpp"

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
         * @brief Commit the current changes to the store, this will apply any
         * pending changes to the underlying data storage for options, and will
         * update the ID remapping as needed based on the changes that were
         * committed, allowing for a structured way to finalize changes to the
         * option data and ensure that the store's state is consistent with the
         * underlying data storage after changes have been made.
         *
         * @param reMap The mapping of original InstrumentIds to their remapped
         * InstrumentIds that should be applied during the commit operation,
         * this allows the commit process to update the ID remapping based on
         * the changes being committed, ensuring that any new or modified
         * options have their IDs correctly remapped as part of the commit
         * process.
         */
        virtual void commit(const IdIdMap<InstrumentId>& reMap) = 0;
    };
}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_OPTION_STORE_HPP__