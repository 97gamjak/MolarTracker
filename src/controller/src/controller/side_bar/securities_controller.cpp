#include "securities_controller.hpp"

#include <qpushbutton.h>
#include <qstackedwidget.h>

#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>

#include "finance/instrument/securities_filter.hpp"
#include "finance/instrument/stock.hpp"
#include "finance/watchlist.hpp"
#include "logging/log_macros.hpp"
#include "mapper/stock_mapper.hpp"
#include "store/i_stock_store.hpp"
#include "store/i_watchlist_store.hpp"
#include "ui/securities/stock_info_model.hpp"
#include "ui/securities/stock_overview.hpp"
#include "ui/securities/ticker_lookup.hpp"
#include "ui/side_bar/securities_category.hpp"
#include "ui/side_bar/side_bar_item.hpp"
#include "ui/side_bar/watchlist_item.hpp"
#include "ui/utils/error.hpp"

REGISTER_LOG_CATEGORY("UI.Controller.SecuritiesSideBarController");

namespace controller
{
    /**
     * @brief Construct a new Securities Side Bar Controller:: Securities Side
     * Bar Controller object
     *
     * @param mainWindow
     * @param stockStore
     * @param watchlistStore
     * @param stackedWidget
     */
    SecuritiesSideBarController::SecuritiesSideBarController(
        QMainWindow*                                   mainWindow,
        const std::shared_ptr<store::IStockStore>&     stockStore,
        const std::shared_ptr<store::IWatchlistStore>& watchlistStore,
        QStackedWidget*                                stackedWidget
    )
        : SideBarCategoryController(new ui::SecuritiesCategory(), mainWindow),
          _stockOverviewWidget(new ui::StockOverviewWidget()),
          _tickerLookupWidget(new ui::TickerLookupWidget()),
          _stockStore(stockStore),
          _watchlistStore(watchlistStore),
          _stackedWidget(stackedWidget)
    {
        _stackedWidget->addWidget(_stockOverviewWidget);

        connect(
            _tickerLookupWidget->getFindButton(),
            &QPushButton::clicked,
            this,
            &SecuritiesSideBarController::_onFindTickerButtonClicked
        );

        connect(
            _tickerLookupWidget->getAcceptButton(),
            &QPushButton::clicked,
            this,
            &SecuritiesSideBarController::_onAcceptTickerButtonClicked
        );

        connect(
            _stockOverviewWidget,
            &ui::StockOverviewWidget::aboutToShowContextMenuForSymbol,
            this,
            &SecuritiesSideBarController::_onAboutToShowTableContextMenu
        );

        connect(
            _stockOverviewWidget,
            &ui::StockOverviewWidget::addToWatchlistRequested,
            this,
            &SecuritiesSideBarController::_onAddToWatchlistRequested
        );

        connect(
            _stockOverviewWidget,
            &ui::StockOverviewWidget::removeFromWatchlistRequested,
            this,
            &SecuritiesSideBarController::_onRemoveFromWatchlistRequested
        );
    }

    /**
     * @brief Handle an item being clicked in the securities category, this will
     * determine which item was clicked and perform the appropriate action, such
     * as opening the corresponding page in the central stack
     *
     * @param item The item that was clicked, this should be a pointer to a
     * SideBarItem that is currently in the category, and will be used to
     * determine which item was clicked and what action to perform
     */
    void SecuritiesSideBarController::_onItemClicked(ui::SideBarItem* item)
    {
        if (item == nullptr)
        {
            LOG_ERROR(
                "SecuritiesSideBarController::_onItemClicked called with null "
                "item"
            );
            return;
        }

        switch (item->getType())
        {
            case ui::SideBarItemType::AllSecuritiesItem:
                _onAllSecuritiesSelected();
                break;
            case ui::SideBarItemType::WatchlistItem:
            {
                const auto* watchlistItem =
                    dynamic_cast<ui::WatchlistItem*>(item);
                _onWatchlistSelected(watchlistItem->getId());
                break;
            }
            case ui::SideBarItemType::SecuritiesCategory:
            case ui::SideBarItemType::OverviewCategory:
            case ui::SideBarItemType::TransactionCategory:
            case ui::SideBarItemType::AccountsItem:
            case ui::SideBarItemType::AccountCategory:
                LOG_ERROR(
                    "SecuritiesSideBarController::_onItemClicked called with "
                    "unexpected item type: " +
                    ui::SideBarItemTypeMeta::toString(item->getType())
                );
                break;
        }
    }

    /**
     * @brief Handle a context menu action being triggered for an item in the
     * securities category, this will determine which item the action was
     * triggered for and which action was triggered, and perform the appropriate
     * action based on that information
     *
     * @param item The item for which the context menu action was triggered,
     * this should be a pointer to a SideBarItem that is currently in the
     * category, and will be used to determine which item the action was
     * triggered for
     * @param action The action that was triggered, this should be a pointer to
     * a QAction that is currently in the context menu for the item, and will be
     * used to determine which action was triggered
     */
    void SecuritiesSideBarController::_onContextMenuRequested(
        ui::SideBarItem* item,
        const QAction*   action
    )
    {
        if (item == nullptr || action == nullptr)
        {
            LOG_ERROR(
                "SecuritiesSideBarController::_onContextMenuRequested called "
                "with null item or action"
            );
            return;
        }

        switch (item->getType())
        {
            case ui::SideBarItemType::SecuritiesCategory:
            {
                const auto* category =
                    dynamic_cast<ui::SecuritiesCategory*>(item);
                _handleContextMenuAction(category, action);
                break;
            }
            case ui::SideBarItemType::WatchlistItem:
            {
                const auto* watchlistItem =
                    dynamic_cast<ui::WatchlistItem*>(item);
                _handleWatchlistContextMenuAction(watchlistItem, action);
                break;
            }
            case ui::SideBarItemType::AllSecuritiesItem:
            case ui::SideBarItemType::OverviewCategory:
            case ui::SideBarItemType::TransactionCategory:
            case ui::SideBarItemType::AccountsItem:
            case ui::SideBarItemType::AccountCategory:
                LOG_ERROR(
                    "SecuritiesSideBarController::_onContextMenuRequested "
                    "called with unexpected item type: " +
                    ui::SideBarItemTypeMeta::toString(item->getType())
                );
                break;
        }
    }

    /**
     * @brief Refresh the securities overview, rebuilding the sidebar's
     * watchlist child rows from the watchlist store.
     *
     */
    void SecuritiesSideBarController::refresh()
    {
        auto* category = dynamic_cast<ui::SecuritiesCategory*>(getCategory());
        if (category == nullptr)
            return;

        category->clearWatchlists();

        for (const auto& watchlist : _watchlistStore->getAllWatchlists())
        {
            category->addWatchlist(
                watchlist.getId(),
                QString::fromStdString(watchlist.getName())
            );
        }
    }

    /**
     * @brief Show securities matching the given filter in the stock
     * overview widget.
     *
     * @param filter
     */
    void SecuritiesSideBarController::_showSecurities(
        const finance::SecuritiesFilter& filter
    )
    {
        const auto stocks = mapper::StockMapper::toStockInfoDrafts(
            _stockStore->getStocks(filter)
        );

        _stockOverviewWidget->getModel()->setRows(stocks);
        _stackedWidget->setCurrentWidget(_stockOverviewWidget);
    }

    /**
     * @brief Slot called when "All Securities" is selected, clearing any
     * active watchlist filter.
     *
     */
    void SecuritiesSideBarController::_onAllSecuritiesSelected()
    {
        _activeWatchlistId = std::nullopt;

        finance::SecuritiesFilter filter;

        _showSecurities(filter);
    }

    /**
     * @brief Slot called when a watchlist is selected, filtering the
     * securities view to that watchlist's symbols.
     *
     * @param id
     */
    void SecuritiesSideBarController::_onWatchlistSelected(WatchlistId id)
    {
        const auto watchlist = _watchlistStore->getWatchlist(id);
        if (!watchlist)
        {
            LOG_ERROR("Watchlist not found: " + id.toString());
            return;
        }

        _activeWatchlistId = id;

        finance::SecuritiesFilter filter;
        filter.symbols = watchlist->getSymbols();

        _showSecurities(filter);
    }

    /**
     * @brief Add a symbol to a watchlist, refreshing the view if that
     * watchlist is currently active.
     *
     * @param symbol
     * @param target
     */
    void SecuritiesSideBarController::_onAddToWatchlist(
        const std::string& symbol,
        WatchlistId        target
    )
    {
        const auto result = _watchlistStore->addSymbol(target, symbol);

        if (!result)
        {
            LOG_ERROR(result.error().toString());
            ui::ErrorDialog::show(
                result.error(),
                "Failed to add symbol to watchlist",
                _stackedWidget
            );
            return;
        }

        if (_activeWatchlistId == target)
            _onWatchlistSelected(target);
    }

    /**
     * @brief Remove a symbol from a watchlist, refreshing the view if that
     * watchlist is currently active.
     *
     * @param symbol
     * @param target
     */
    void SecuritiesSideBarController::_onRemoveFromWatchlist(
        const std::string& symbol,
        WatchlistId        target
    )
    {
        const auto result = _watchlistStore->removeSymbol(target, symbol);

        if (!result)
        {
            LOG_ERROR(result.error().toString());
            ui::ErrorDialog::show(
                result.error(),
                "Failed to remove symbol from watchlist",
                _stackedWidget
            );
            return;
        }

        if (_activeWatchlistId == target)
            _onWatchlistSelected(target);
    }

    /**
     * @brief Create a new, empty watchlist and refresh the sidebar.
     *
     * @param name
     */
    void SecuritiesSideBarController::_onCreateWatchlist(
        const std::string& name
    )
    {
        static_cast<void>(_watchlistStore->createWatchlist(name));
        refresh();
    }

    /**
     * @brief Delete a watchlist, falling back to "All Securities" if it was
     * the active filter, then refresh the sidebar.
     *
     * @param id
     */
    void SecuritiesSideBarController::_onDeleteWatchlist(WatchlistId id)
    {
        if (_activeWatchlistId == id)
            _onAllSecuritiesSelected();

        _watchlistStore->deleteWatchlist(id);
        refresh();
    }

    /**
     * @brief Rename a watchlist and refresh the sidebar.
     *
     * @param id
     * @param newName
     */
    void SecuritiesSideBarController::_onRenameWatchlist(
        WatchlistId        id,
        const std::string& newName
    )
    {
        _watchlistStore->renameWatchlist(id, newName);
        refresh();
    }

    /**
     * @brief Handle context menu actions for the securities category.
     *
     * @param category The securities category
     * @param action The action triggered
     */
    void SecuritiesSideBarController::_handleContextMenuAction(
        const ui::SecuritiesCategory* category,
        const QAction*                action
    )
    {
        if (action == category->getCreateAction())
        {
            onCreateStock("");
        }
        else if (action == category->getCreateWatchlistAction())
        {
            const auto name = QInputDialog::getText(
                                  _stackedWidget,
                                  "Create Watchlist",
                                  "Watchlist name:"
            )
                                  .trimmed();

            if (!name.isEmpty())
                _onCreateWatchlist(name.toStdString());
        }
        else
        {
            throw std::logic_error("Context menu action not handled");
        }
    }

    /**
     * @brief Handle context menu actions for a single watchlist item
     * (Rename/Delete).
     *
     * @param item The watchlist item
     * @param action The action triggered
     */
    void SecuritiesSideBarController::_handleWatchlistContextMenuAction(
        const ui::WatchlistItem* item,
        const QAction*           action
    )
    {
        if (item == nullptr)
            return;

        if (action == item->getRenameAction())
        {
            const auto newName = QInputDialog::getText(
                                     _stackedWidget,
                                     "Rename Watchlist",
                                     "Watchlist name:",
                                     QLineEdit::Normal,
                                     item->text()
            )
                                     .trimmed();

            if (!newName.isEmpty())
                _onRenameWatchlist(item->getId(), newName.toStdString());
        }
        else if (action == item->getDeleteAction())
        {
            const auto confirmed = QMessageBox::question(
                _stackedWidget,
                "Delete Watchlist",
                "Delete watchlist '" + item->text() +
                    "'? This cannot be undone."
            );

            if (confirmed == QMessageBox::Yes)
                _onDeleteWatchlist(item->getId());
        }
        else
        {
            throw std::logic_error("Watchlist context menu action not handled");
        }
    }

    /**
     * @brief Slot called when the find ticker button is clicked.
     *
     */
    void SecuritiesSideBarController::_onFindTickerButtonClicked()
    {
        const auto ticker = _tickerLookupWidget->getTickerInput();

        const auto result = finance::Stock::retrieveTickerInfo(ticker);

        if (!result)
        {
            _tickerLookupWidget->displayError(result.error().toString());
            _acceptedQuote = std::nullopt;
            return;
        }

        _acceptedQuote = result.value();
        _tickerLookupWidget->displayQuote(
            mapper::StockMapper::toStockInfoDraft(result.value())
        );
    }

    /**
     * @brief Slot called when the accept ticker button is clicked.
     *
     */
    void SecuritiesSideBarController::_onAcceptTickerButtonClicked()
    {
        if (_acceptedQuote)
        {
            const auto result = _stockStore->addStock(_acceptedQuote.value());

            if (result != store::StockStoreResult::Ok)
                _tickerLookupWidget->displayError("Failed to add stock");
        }

        _tickerLookupWidget->clearResult();
        _tickerLookupWidget->hide();

        if (_acceptedQuote)
            emit stockCreated(_acceptedQuote.value());
    }

    /**
     * @brief Create a new stock with the given ticker symbol.
     *
     * @param ticker The ticker symbol of the stock to create
     */
    void SecuritiesSideBarController::onCreateStock(const std::string& ticker)
    {
        _tickerLookupWidget->setTicker(ticker);
        _tickerLookupWidget->show();
    }

    /**
     * @brief Slot called just before the securities table's context menu is
     * shown, refreshing the widget's watchlist data from the store.
     *
     * @param symbol The ticker symbol of the right-clicked row (unused
     * here, the menu itself carries it back via the widget's signals).
     */
    void SecuritiesSideBarController::_onAboutToShowTableContextMenu(
        const std::string& /*symbol*/
    )
    {
        std::vector<std::pair<WatchlistId, QString>> watchlists;
        for (const auto& watchlist : _watchlistStore->getAllWatchlists())
        {
            watchlists.emplace_back(
                watchlist.getId(),
                QString::fromStdString(watchlist.getName())
            );
        }

        _stockOverviewWidget->setAvailableWatchlists(watchlists);
        _stockOverviewWidget->setActiveWatchlist(_activeWatchlistId);
    }

    /**
     * @brief Slot called when the securities table's context menu requests
     * adding a symbol to a watchlist.
     *
     * @param symbol
     * @param target
     */
    void SecuritiesSideBarController::_onAddToWatchlistRequested(
        const std::string& symbol,
        WatchlistId        target
    )
    {
        _onAddToWatchlist(symbol, target);
    }

    /**
     * @brief Slot called when the securities table's context menu requests
     * removing a symbol from a watchlist.
     *
     * @param symbol
     * @param target
     */
    void SecuritiesSideBarController::_onRemoveFromWatchlistRequested(
        const std::string& symbol,
        WatchlistId        target
    )
    {
        _onRemoveFromWatchlist(symbol, target);
    }

}   // namespace controller
