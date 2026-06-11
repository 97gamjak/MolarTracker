#ifndef __STORE__SRC__STORE__STOCK_STORE_HPP__
#define __STORE__SRC__STORE__STOCK_STORE_HPP__

#include <memory>
#include <unordered_map>

#include "config/id_types.hpp"
#include "config/signal_tags.hpp"
#include "config/strong_id.hpp"
#include "finance/instrument/stock.hpp"
#include "service/i_instrument_service.hpp"
#include "store/base/base_store.hpp"
#include "store/i_stock_store.hpp"

namespace store
{
    /**
     * @brief Store for managing stocks
     *
     */
    class StockStore : public BaseStore<finance::Stock, StockId>,
                       public IStockStore
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
        unorderedIdMap<InstrumentId, InstrumentId> _instrumentIdMap;

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
        StockStoreResult addStock(finance::Stock stock) override;

        [[nodiscard]]
        finance::Stocks getStocks(
            const idSet<InstrumentId>& ids
        ) const override;

        [[nodiscard]]
        finance::Stocks getStocks() const override;

        [[nodiscard]]
        std::optional<finance::Stock> getStock(InstrumentId id) const override;

        [[nodiscard]]
        std::vector<std::string> getAllTickers() const override;

        [[nodiscard]]
        std::unordered_map<std::string, InstrumentId> getTickerMap() const;

        [[nodiscard]]
        unorderedIdMap<InstrumentId, std::string> getInstrumentIdToNameMap(
        ) const override;

        [[nodiscard]]
        bool stockExists(const std::string& ticker, bool checkDeleted) const;

        [[nodiscard]]
        std::optional<InstrumentId> getInstrumentId(
            const std::string& ticker
        ) const override;

        void commit() override;

        [[nodiscard]]
        const unorderedIdMap<InstrumentId, InstrumentId>& getInstrumentIdMap(
        ) const override;

        [[nodiscard]]
        Connection subscribeToStoreChange(
            StoreChanged<StockId>::func func,
            void*                       subscriber
        ) override;
    };
}   // namespace store

#endif   // __STORE__SRC__STORE__STOCK_STORE_HPP__