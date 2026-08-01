#ifndef __UI__INCLUDE__UI__SECURITIES__STOCK_OVERVIEW_HPP__
#define __UI__INCLUDE__UI__SECURITIES__STOCK_OVERVIEW_HPP__

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <QString>
#include <QWidget>

#include "config/id_types.hpp"

class QTableView;
class QSortFilterProxyModel;
class QPoint;

namespace ui
{
    class StockInfoTableModel;

    /**
     * @brief Widget for displaying stock overview information.
     */
    class StockOverviewWidget : public QWidget
    {
        Q_OBJECT

       private:
        /// The model for displaying stock information
        StockInfoTableModel* _model;
        /// The proxy model for filtering and sorting
        QSortFilterProxyModel* _proxy;
        /// The table view for displaying stock information
        QTableView* _table;

        /// Watchlists available to add the right-clicked symbol to,
        /// supplied by the controller just before the context menu is shown
        std::vector<std::pair<WatchlistId, QString>> _availableWatchlists;
        /// The watchlist currently filtering the view, if any, used to show
        /// the "Remove from current watchlist" context menu entry
        std::optional<WatchlistId> _activeWatchlistId = std::nullopt;

       public:
        explicit StockOverviewWidget(QWidget* parent = nullptr);

        [[nodiscard]]
        StockInfoTableModel* getModel() const;

        void setAvailableWatchlists(
            const std::vector<std::pair<WatchlistId, QString>>& watchlists
        );
        void setActiveWatchlist(std::optional<WatchlistId> id);

       signals:
        /**
         * @brief Emitted when a stock ticker is selected.
         *
         * @param ticker The selected stock ticker.
         */
        void tickerSelected(const std::string& ticker);

        /**
         * @brief Emitted synchronously right before the context menu for a
         * table row is built, so the controller can refresh
         * setAvailableWatchlists()/setActiveWatchlist() with up-to-date
         * data before the menu is shown.
         *
         * @param symbol The ticker symbol of the right-clicked row.
         */
        void aboutToShowContextMenuForSymbol(const std::string& symbol);

        /**
         * @brief Emitted when the user chooses to add a symbol to a
         * watchlist from the context menu.
         *
         * @param symbol The ticker symbol.
         * @param target The watchlist to add the symbol to.
         */
        void addToWatchlistRequested(
            const std::string& symbol,
            WatchlistId        target
        );

        /**
         * @brief Emitted when the user chooses to remove a symbol from the
         * currently active watchlist via the context menu.
         *
         * @param symbol The ticker symbol.
         * @param target The watchlist to remove the symbol from.
         */
        void removeFromWatchlistRequested(
            const std::string& symbol,
            WatchlistId        target
        );

       private:
        void _setupTable();
        void _onSelectionChanged();
        void _showContextMenu(const QPoint& pos);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SECURITIES__STOCK_OVERVIEW_HPP__
