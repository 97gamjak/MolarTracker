#ifndef __STORE__INCLUDE__STORE__I_OPTION_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_OPTION_STORE_HPP__

#include <cstdint>

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

    class IOptionStore
    {
       public:
        virtual ~IOptionStore() = default;

        [[nodiscard]]
        virtual OptionStoreResult addOption(finance::Option option) = 0;

        [[nodiscard]]
        virtual const IdIdMap<InstrumentId>& getInstrumentIdMap() const = 0;

        virtual void commit() = 0;
    };
}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_OPTION_STORE_HPP__