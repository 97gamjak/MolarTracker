#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__

#include <QObject>
#include <optional>

#include "config/id_types.hpp"
#include "finance/instrument/stock.hpp"
#include "side_bar_category_controller.hpp"

namespace finance
{
    class SecuritiesFilter;   // Forward declaration
}   // namespace finance

namespace ui
{
    class StockOverviewWidget;   // Forward declaration
    class SecuritiesCategory;    // Forward declaration
    class TickerLookupWidget;    // Forward declaration
    class WatchlistItem;         // Forward declaration
}   // namespace ui

namespace store
{
    class IStockStore;       // Forward declaration
    class IWatchlistStore;   // Forward declaration
}   // namespace store

class QStackedWidget;   // Forward declaration
class QAction;          // Forward declaration

namespace controller
{

    /**
     * @brief Controller for the securities side bar.
     *
     * This class manages the interactions between the UI components and the
     * underlying data model for the securities side bar.
     */
    class SecuritiesSideBarController : public SideBarCategoryController
    {
        Q_OBJECT

       private:
        /// Widget for displaying stock overview infos
        ui::StockOverviewWidget* _stockOverviewWidget = nullptr;
        /// Widget for looking up stock tickers
        ui::TickerLookupWidget* _tickerLookupWidget = nullptr;

        /// Optional accepted stock quote
        std::optional<finance::Stock> _acceptedQuote = std::nullopt;

        /// Reference to the stock store
        std::shared_ptr<store::IStockStore> _stockStore;
        /// Reference to the watchlist store
        std::shared_ptr<store::IWatchlistStore> _watchlistStore;

        /// Pointer to the stacked widget
        QStackedWidget* _stackedWidget;

        /// The watchlist currently used to filter the securities view, if
        /// any (nullopt means "All Securities")
        std::optional<WatchlistId> _activeWatchlistId = std::nullopt;

       public:
        explicit SecuritiesSideBarController(
            QMainWindow*                                   mainWindow,
            const std::shared_ptr<store::IStockStore>&     stockStore,
            const std::shared_ptr<store::IWatchlistStore>& watchlistStore,
            QStackedWidget*                                stackedWidget
        );

        void refresh() override;

        void onCreateStock(const std::string& ticker);

       signals:
        /**
         * @brief Emitted when a new stock is created
         *
         * @param stock The stock that was created
         */
        void stockCreated(const finance::Stock& stock);

       private:
        void _onItemClicked(ui::SideBarItem* item) override;
        void _onContextMenuRequested(
            ui::SideBarItem* item,
            const QAction*   action
        ) override;

        void _onAllSecuritiesSelected();
        void _onWatchlistSelected(WatchlistId id);

        void _onAddToWatchlist(const std::string& symbol, WatchlistId target);
        void _onRemoveFromWatchlist(
            const std::string& symbol,
            WatchlistId        target
        );

        void _onCreateWatchlist(const std::string& name);
        void _onDeleteWatchlist(WatchlistId id);
        void _onRenameWatchlist(WatchlistId id, const std::string& newName);

        void _handleContextMenuAction(
            const ui::SecuritiesCategory* category,
            const QAction*                action
        );

        void _handleWatchlistContextMenuAction(
            const ui::WatchlistItem* item,
            const QAction*           action
        );

        void _onFindTickerButtonClicked();
        void _onAcceptTickerButtonClicked();

        void _onAboutToShowTableContextMenu(const std::string& symbol);
        void _onAddToWatchlistRequested(
            const std::string& symbol,
            WatchlistId        target
        );
        void _onRemoveFromWatchlistRequested(
            const std::string& symbol,
            WatchlistId        target
        );

        void _showSecurities(const finance::SecuritiesFilter& filter);
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__
