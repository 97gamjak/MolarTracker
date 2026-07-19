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
    /**
     * @brief Store for managing options, this class implements the IOptionStore
     * interface and provides methods to add options, retrieve the mapping of
     * instrument IDs, commit changes, and check if an option exists, allowing
     * for a structured way to manage options within the application and
     * interact with the underlying data storage for options.
     *
     */
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
        std::expected<InstrumentId, OptionStoreResult> addOption(
            finance::Option option
        ) override;

        [[nodiscard]]
        const IdIdMap<InstrumentId>& getInstrumentIdMap() const override;

        void commit(const IdIdMap<InstrumentId>& reMap);

        void reload() override;

        [[nodiscard]]
        bool optionExists(const finance::Option& option) const;

       private:
        void _onInstrumentIdRemap(const IdIdMap<InstrumentId>& reMap);
    };
}   // namespace store

#endif   // __STORE__SRC__STORE__OPTION_STORE_HPP__