#ifndef __STORE__INCLUDE__STORE__STOCK_STORE_HPP__
#define __STORE__INCLUDE__STORE__STOCK_STORE_HPP__

#include <memory>
#include <unordered_map>

#include "base/base_store.hpp"
#include "config/id_types.hpp"
#include "config/signal_tags.hpp"
#include "finance/instrument/stock.hpp"
#include "service/i_instrument_service.hpp"

namespace store
{
    /**
     * @brief Result of stock store operations
     *
     */
    enum class StockStoreResult : std::uint8_t
    {
        Ok,
        Error,
        StockAlreadyExists,
        StockNotFound,
    };

    /**
     * @brief Store for managing stocks
     *
     */
    class StockStore : public BaseStore<finance::Stock, StockId>
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
        Observable<OnIdRemap<InstrumentId>> _onInstrumentIdRemap;

       public:
        explicit StockStore(
            InstrumentServicePtr instrumentService,
            InstrumentIdSeq&     instrumentIdSeq
        );

        ~StockStore() override                   = default;
        StockStore(const StockStore&)            = delete;
        StockStore& operator=(const StockStore&) = delete;
        StockStore(StockStore&&)                 = delete;
        StockStore& operator=(StockStore&&)      = delete;

        [[nodiscard]]
        StockStoreResult addStock(finance::Stock stock);

        [[nodiscard]]
        std::vector<finance::Stock> getStocks(
            const idSet<InstrumentId>& ids = {}
        ) const;
        [[nodiscard]]
        std::optional<finance::Stock> getStock(InstrumentId id) const;

        [[nodiscard]]
        std::vector<std::string> getAllTickers() const;

        [[nodiscard]]
        std::unordered_map<std::string, InstrumentId> getTickerMap() const;

        [[nodiscard]]
        instrumentMap<std::string> getInstrumentIdToNameMap() const;

        [[nodiscard]]
        bool stockExists(const std::string& ticker, bool checkDeleted) const;

        [[nodiscard]]
        std::optional<InstrumentId> getInstrumentId(
            const std::string& ticker
        ) const;

        void commit();

        Connection subscribeToInstrumentIdRemap(
            OnIdRemap<InstrumentId>::func func,
            void*                         userData
        );
    };
}   // namespace store

#endif   // __STORE__INCLUDE__STORE__STOCK_STORE_HPP__