#include "ui/securities/stock_overview.hpp"

#include <qboxlayout.h>
#include <qheaderview.h>
#include <qsortfilterproxymodel.h>
#include <qtableview.h>

#include <QLineEdit>
#include <QMap>
#include <QMenu>

#include "common/qt_helpers.hpp"
#include "ui/securities/stock_info_model.hpp"

using common::makeQChild;

namespace ui
{
    /**
     * @brief Construct a new Stock Overview Widget:: Stock Overview Widget
     * object
     *
     * @param parent
     */
    StockOverviewWidget::StockOverviewWidget(QWidget* parent)
        : QWidget(parent),
          _model(new StockInfoTableModel(this)),
          _proxy(new QSortFilterProxyModel(this)),
          _table(new QTableView(this))
    {
        _proxy->setSourceModel(_model);
        _proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        _proxy->setFilterKeyColumn(-1);   // search across all columns

        auto* search = makeQChild<QLineEdit>(this);
        search->setPlaceholderText("Search instruments…");
        search->setClearButtonEnabled(true);

        connect(
            search,
            &QLineEdit::textChanged,
            _proxy,
            &QSortFilterProxyModel::setFilterFixedString
        );

        _setupTable();

        auto* topBar = makeQChild<QHBoxLayout>();
        topBar->addWidget(search);
        topBar->addStretch();

        auto* layout = makeQChild<QVBoxLayout>(this);
        layout->addLayout(topBar);
        layout->addWidget(_table);
    }

    /**
     * @brief Set up the table view for displaying stock information.
     */
    void StockOverviewWidget::_setupTable()
    {
        _table->setModel(_proxy);
        _table->setSortingEnabled(true);
        _table->setSelectionBehavior(QAbstractItemView::SelectRows);
        _table->setSelectionMode(QAbstractItemView::SingleSelection);
        _table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        _table->setAlternatingRowColors(true);
        _table->verticalHeader()->hide();
        _table->setContextMenuPolicy(Qt::CustomContextMenu);

        auto* hdr = _table->horizontalHeader();
        hdr->setSortIndicatorShown(true);
        hdr->setStretchLastSection(false);

        // Default sort: alphabetical by ticker
        _table->sortByColumn(
            StockInfoTableModel::getTickerColumn(),
            Qt::AscendingOrder
        );

        connect(
            _table->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &StockOverviewWidget::_onSelectionChanged
        );

        connect(
            _table,
            &QTableView::customContextMenuRequested,
            this,
            &StockOverviewWidget::_showContextMenu
        );
    }

    /**
     * @brief Handle selection changes in the table view.
     */
    void StockOverviewWidget::_onSelectionChanged()
    {
        const auto selected = _table->selectionModel()->selectedRows();
        if (selected.isEmpty())
            return;

        const auto    sourceIndex = _proxy->mapToSource(selected.first());
        const QString ticker =
            _model
                ->data(
                    _model->index(
                        sourceIndex.row(),
                        StockInfoTableModel::getTickerColumn()
                    ),
                    Qt::DisplayRole
                )
                .toString();

        emit tickerSelected(ticker.toStdString());
    }

    /**
     * @brief Show the context menu for a right-clicked security row, letting
     * the user add/remove the symbol to/from a watchlist. Watchlist data is
     * never fetched from a store here — the widget only ever consumes data
     * the controller supplies via setAvailableWatchlists()/
     * setActiveWatchlist(), keeping the widget store-agnostic.
     *
     * @param pos The position of the right-click, in the table's viewport
     * coordinates.
     */
    void StockOverviewWidget::_showContextMenu(const QPoint& pos)
    {
        const auto index = _table->indexAt(pos);
        if (!index.isValid())
            return;

        const auto sourceIndex = _proxy->mapToSource(index);
        const QString ticker =
            _model
                ->data(
                    _model->index(
                        sourceIndex.row(),
                        StockInfoTableModel::getTickerColumn()
                    ),
                    Qt::DisplayRole
                )
                .toString();
        const auto symbol = ticker.toStdString();

        // synchronous "pull": lets the controller refresh
        // _availableWatchlists/_activeWatchlistId with up-to-date data
        // before the menu below is built
        emit aboutToShowContextMenuForSymbol(symbol);

        auto* menu    = utils::makeQChild<QMenu>(this);
        auto* addMenu = menu->addMenu("Add to Watchlist");

        QMap<QAction*, WatchlistId> addActions;
        for (const auto& [id, name] : _availableWatchlists)
            addActions[addMenu->addAction(name)] = id;

        QAction* removeAction = nullptr;
        if (_activeWatchlistId)
            removeAction = menu->addAction("Remove from current watchlist");

        if (menu->actions().isEmpty())
            return;

        QAction* chosen = menu->exec(_table->viewport()->mapToGlobal(pos));
        if (chosen == nullptr)
            return;

        if (addActions.contains(chosen))
            emit addToWatchlistRequested(symbol, addActions[chosen]);
        else if (chosen == removeAction && _activeWatchlistId)
            emit removeFromWatchlistRequested(symbol, *_activeWatchlistId);
    }

    /**
     * @brief Get the model for displaying stock information.
     *
     * @return StockInfoTableModel* The model for displaying stock information.
     */
    StockInfoTableModel* StockOverviewWidget::getModel() const
    {
        return _model;
    }

    /**
     * @brief Set the watchlists available in the "Add to Watchlist" context
     * menu submenu.
     *
     * @param watchlists Pairs of (id, display name)
     */
    void StockOverviewWidget::setAvailableWatchlists(
        const std::vector<std::pair<WatchlistId, QString>>& watchlists
    )
    {
        _availableWatchlists = watchlists;
    }

    /**
     * @brief Set the watchlist currently filtering the view, controlling
     * whether "Remove from current watchlist" is shown in the context menu.
     *
     * @param id The active watchlist, or nullopt if "All Securities" is
     * active.
     */
    void StockOverviewWidget::setActiveWatchlist(
        std::optional<WatchlistId> id
    )
    {
        _activeWatchlistId = id;
    }

}   // namespace ui
