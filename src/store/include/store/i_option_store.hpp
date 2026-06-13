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

        [[nodiscard]]
        virtual std::expected<InstrumentId, OptionStoreResult> addOption(
            finance::Option option
        ) = 0;

        [[nodiscard]]
        virtual const IdIdMap<InstrumentId>& getInstrumentIdMap() const = 0;

        virtual void commit(const IdIdMap<InstrumentId>& reMap) = 0;
    };
}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_OPTION_STORE_HPP__