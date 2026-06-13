#ifndef __STORE__SRC__STORE__OPTION_STORE_HPP__
#define __STORE__SRC__STORE__OPTION_STORE_HPP__

#include <memory>

#include "config/id_types.hpp"
#include "finance/instrument/option.hpp"
#include "service/i_instrument_service.hpp"
#include "store/base/base_store.hpp"
#include "store/i_option_store.hpp"
#include "utils/container/id_id_map.hpp"

namespace store
{
    class OptionStore : public BaseStore<finance::Option, OptionId>,
                        public IOptionStore
    {
       private:
        /// The type of the Instrument service pointer
        using InstrumentServicePtr =
            std::shared_ptr<service::IInstrumentService>;

        /// The Instrument service
        InstrumentServicePtr _instrumentService;

        /// The instrument ID sequence
        InstrumentIdSeq& _instrumentIdSeq;

        /// The observable for instrument ID remapping events
        IdIdMap<InstrumentId> _instrumentIdMap;

       public:
        explicit OptionStore(
            InstrumentServicePtr instrumentService,
            InstrumentIdSeq&     instrumentIdSeq
        );

        ~OptionStore() override                    = default;
        OptionStore(const OptionStore&)            = delete;
        OptionStore& operator=(const OptionStore&) = delete;
        OptionStore(OptionStore&&)                 = delete;
        OptionStore& operator=(OptionStore&&)      = delete;

        [[nodiscard]]
        OptionStoreResult addOption(finance::Option option) override;

        [[nodiscard]]
        const IdIdMap<InstrumentId>& getInstrumentIdMap() const override;

        void commit() override;

        [[nodiscard]]
        bool optionExists(const finance::Option& option) const;
    };
}   // namespace store

#endif   // __STORE__SRC__STORE__OPTION_STORE_HPP__