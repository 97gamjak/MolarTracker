#include "position_controller.hpp"

#include <QFuture>
#include <QTimer>
#include <QtConcurrent>
#include <memory>

#include "finance/price_cache.hpp"

namespace controller
{
    PositionController::PositionController(
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<store::ITransactionStore>& transactionStore
    )
        : _pollTimer(new QTimer()),
          _priceCache(std::make_unique<finance::PriceCache>()),
          _positionStore(positionStore),
          _transactionStore(transactionStore),
          _expectedSymbolCount(0)
    {
        connect(
            _priceWatcher,
            &QFutureWatcher<
                std::unordered_map<std::string, finance::PriceQuote>>::finished,
            this,
            &PositionController::_onPricesFetched
        );

        connect(
            _pollTimer,
            &QTimer::timeout,
            this,
            &PositionController::_fetchPrices
        );

        _pollTimer->setInterval(60'000);

        _fetchPrices();
        _pollTimer->start();
    }

    void PositionController::_fetchPrices()
    {
        if (_priceWatcher.isRunning())
            return;   // don't stack concurrent fetches

        auto tickers = _collectTickers();

        if (tickers.empty())
            return;

        _expectedSymbolCount = tickers.size();

        _priceWatcher.setFuture(
            QtConcurrent::run(
                [_tickers = std::move(tickers)]()
                { return finance::PriceFeedService::fetchBatch(_tickers); }
            )
        );
    }

    void PositionController::_onPricesFetched()
    {
        const auto result = _priceWatcher.result();
        // Gate: only update if we got back the full symbol set
        if (result.size() == _expectedSymbolCount)
            _priceCache->update(result);

        // Notify table model — adjust column indices to your price/P&L range
        emit _tableModel->dataChanged(...);
    }

}   // namespace controller
