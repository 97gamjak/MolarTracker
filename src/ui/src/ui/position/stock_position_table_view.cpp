#include "ui/position/stock_position_table_view.hpp"

#include <QHeaderView>
#include <QSortFilterProxyModel>

#include "ui/position/stock_position_table_model.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Construct a new StockPositionTableView object
     *
     * @param parent
     */
    StockPositionTableView::StockPositionTableView(QWidget* parent)
        : QTableView{parent}
    {
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setSelectionMode(QAbstractItemView::SingleSelection);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
        setAlternatingRowColors(true);
        setSortingEnabled(true);

        horizontalHeader()->setStretchLastSection(false);
        horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        verticalHeader()->setVisible(false);
        const auto defaultSize = 24;
        verticalHeader()->setDefaultSectionSize(defaultSize);
    }

    /**
     * @brief Set the model for the position table view
     *
     * @param model
     */
    void StockPositionTableView::setPositionModel(
        StockPositionTableModel* model
    )
    {
        _proxy = utils::makeQChild<QSortFilterProxyModel>(this);
        _proxy->setSourceModel(model);
        _proxy->setSortRole(Qt::DisplayRole);

        QTableView::setModel(_proxy);
        _setupColumns();

        // Default sort: newest positions first.
        sortByColumn(
            static_cast<int>(StockPositionColumns::OpenedAt),
            Qt::DescendingOrder
        );
    }

    /**
     * @brief Get the currently selected row in the position table view
     *
     * @return int The source-model row index of the current selection, or -1 if
     * nothing is selected.
     */
    int StockPositionTableView::selectedRow() const
    {
        const auto indexes = selectionModel()->selectedRows();
        if (indexes.isEmpty())
            return -1;
        return _proxy->mapToSource(indexes.first()).row();
    }

    /**
     * @brief Set up the columns for the position table view, this will
     * configure the column widths and resizing behavior to ensure that the
     * position data is displayed clearly and is easy to read for the user.
     */
    void StockPositionTableView::_setupColumns()
    {
        auto* header = horizontalHeader();

        // Fixed-width columns.
        header->setSectionResizeMode(
            static_cast<int>(StockPositionColumns::Ticker),
            QHeaderView::Fixed
        );
        const auto tickerWidth = 80;
        setColumnWidth(
            static_cast<int>(StockPositionColumns::Ticker),
            tickerWidth
        );

        header->setSectionResizeMode(
            static_cast<int>(StockPositionColumns::OpenedAt),
            QHeaderView::Fixed
        );
        const auto openedAtWidth = 100;
        setColumnWidth(
            static_cast<int>(StockPositionColumns::OpenedAt),
            openedAtWidth
        );

        // Numeric columns: fixed, right-aligned content already set by model.
        for (const auto col : {
                 StockPositionColumns::Quantity,
                 StockPositionColumns::AvgCost,
                 StockPositionColumns::CostBasis,
                 StockPositionColumns::LastPrice,
                 StockPositionColumns::MarketValue,
                 StockPositionColumns::UnrealizedPnl,
                 StockPositionColumns::UnrealizedPnlPct,
             })
        {
            header->setSectionResizeMode(
                static_cast<int>(col),
                QHeaderView::Fixed
            );
            const auto defaultWidth = 110;
            setColumnWidth(static_cast<int>(col), defaultWidth);
        }

        // Name stretches to fill remaining space.
        header->setSectionResizeMode(
            static_cast<int>(StockPositionColumns::Name),
            QHeaderView::Stretch
        );
    }

}   // namespace ui