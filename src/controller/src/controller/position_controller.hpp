#ifndef __CONTROLLER__SRC__CONTROLLER__POSITION_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__POSITION_CONTROLLER_HPP__

#include <qtmetamacros.h>

#include <QFutureWatcher>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "finance/price_quote.hpp"

class QTimer;   // Forward declaration

namespace store
{
    class IPositionStore;      // Forward declaration
    class ITransactionStore;   // Forward declaration
}   // namespace store

namespace finance
{
    class PriceCache;   // Forward declaration
}   // namespace finance

namespace controller
{
    /**
     * @brief Controller for managing positions and their associated data.
     *
     */
    class PositionController : public QObject
    {
        Q_OBJECT

       private:
        QFutureWatcher<std::unordered_map<std::string, finance::PriceQuote>>
                                                  _priceWatcher;
        QTimer*                                   _pollTimer;
        std::unique_ptr<finance::PriceCache>      _priceCache;
        std::shared_ptr<store::IPositionStore>    _positionStore;
        std::shared_ptr<store::ITransactionStore> _transactionStore;

        std::size_t _expectedSymbolCount;

       public:
        PositionController(
            const std::shared_ptr<store::IPositionStore>&    positionStore,
            const std::shared_ptr<store::ITransactionStore>& transactionStore
        );

       private:
        void _fetchPrices();
        void _onPricesFetched();

        [[nodiscard]]
        std::unordered_set<std::string> _collectTickers() const;
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__POSITION_CONTROLLER_HPP__
