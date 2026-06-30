#ifndef __STORE__SRC__STORE__STOCK_STORE_HPP__
#define __STORE__SRC__STORE__STOCK_STORE_HPP__

#include <memory>
#include <unordered_map>

#include "config/id_types.hpp"
#include "config/signal_tags.hpp"
#include "finance/instrument/stock.hpp"
#include "finance/instrument/stocks.hpp"
#include "service/i_instrument_service.hpp"
#include "store/base/base_store.hpp"
#include "store/i_stock_store.hpp"
#include "utils/container/id_id_map.hpp"

namespace store
{
    /**
     * @brief Store for managing stocks
     *
     */
    class StockStore : public BaseStore<finance::Stock, StockId>,
                       public IStockStore,
                       public IStockStoreReader
    {
       private:
        Observable<OnCommit> _onCommit;
        /// The Instrument service
        std::shared_ptr<service::IInstrumentService> _instrumentService;

        /// The instrument ID sequence
        InstrumentIdSeq& _instrumentIdSeq;

        /// The observable for instrument ID remapping events
        IdIdMap<InstrumentId> _instrumentIdMap;

       public:
        explicit StockStore(
            std::shared_ptr<service::IInstrumentService> instrumentService,
            InstrumentIdSeq&                             instrumentIdSeq
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
            const finance::StockFilter& filter
        ) const override;

        [[nodiscard]]
        std::optional<finance::Stock> getStock(InstrumentId id) const override;

        [[nodiscard]]
        std::optional<finance::Stock> getStock(StockId id) const override;

        [[nodiscard]]
        std::optional<finance::Stock> getStock(
            const std::string& ticker
        ) const override;

        [[nodiscard]]
        std::unordered_map<std::string, InstrumentId> getTickerMap() const;

        [[nodiscard]]
        bool stockExists(const std::string& ticker, bool checkDeleted) const;

        void commit();

        [[nodiscard]]
        const IdIdMap<InstrumentId>& getInstrumentIdMap() const;

        [[nodiscard]]
        Connection subscribeToStockAdded(
            OnStoreItemAdded<finance::Stock>::func func,
            void*                                  subscriber
        ) override;

        [[nodiscard]]
        Connection subscribeToStockUpdated(
            OnStoreItemUpdated<finance::Stock>::func func,
            void*                                    subscriber
        ) override;

        [[nodiscard]]
        Connection subscribeToStockRemoved(
            OnStoreItemRemoved<StockId>::func func,
            void*                             subscriber
        ) override;

        [[nodiscard]]
        Connection subscribeToCommit(
            const OnCommit::func& func,
            void*                 subscriber
        ) override;

       private:
        [[nodiscard]] std::optional<finance::Stock> _getStock(
            const finance::StockFilter& filter
        ) const;

        void _notifyCommit();
    };
}   // namespace store

#endif   // __STORE__SRC__STORE__STOCK_STORE_HPP__