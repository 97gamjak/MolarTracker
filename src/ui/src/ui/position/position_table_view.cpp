#include "ui/position/position_table_view.hpp"

#include <QHeaderView>
#include <QSortFilterProxyModel>

#include "common/qt_helpers.hpp"
#include "ui/position/position_table_model.hpp"

namespace ui
{

    /**
     * @brief Construct a new Position Table View:: Position Table View object
     *
     * @param parent
     */
    PositionTableView::PositionTableView(QWidget* parent) : QTableView{parent}
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
    void PositionTableView::setPositionModel(StockPositionTableModel* model)
    {
        _proxy = common::makeQChild<QSortFilterProxyModel>(this);
        _proxy->setSourceModel(model);
        _proxy->setSortRole(Qt::DisplayRole);

        QTableView::setModel(_proxy);
        _setupColumns();

        // Default sort: newest positions first.
        sortByColumn(
            static_cast<int>(PositionColumns::OpenedAt),
            Qt::DescendingOrder
        );
    }

    /**
     * @brief Get the currently selected row in the position table view
     *
     * @return int The source-model row index of the current selection, or -1 if
     * nothing is selected.
     */
    int PositionTableView::selectedRow() const
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
    void PositionTableView::_setupColumns()
    {
        auto* header = horizontalHeader();

        // Fixed-width columns.
        header->setSectionResizeMode(
            static_cast<int>(PositionColumns::Ticker),
            QHeaderView::Fixed
        );
        const auto tickerWidth = 80;
        setColumnWidth(static_cast<int>(PositionColumns::Ticker), tickerWidth);

        header->setSectionResizeMode(
            static_cast<int>(PositionColumns::OpenedAt),
            QHeaderView::Fixed
        );
        const auto openedAtWidth = 100;
        setColumnWidth(
            static_cast<int>(PositionColumns::OpenedAt),
            openedAtWidth
        );

        // Numeric columns: fixed, right-aligned content already set by model.
        for (const auto col : {
                 PositionColumns::Quantity,
                 PositionColumns::AvgCost,
                 PositionColumns::CostBasis,
                 PositionColumns::LastPrice,
                 PositionColumns::MarketValue,
                 PositionColumns::UnrealizedPnl,
                 PositionColumns::UnrealizedPnlPct,
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
            static_cast<int>(PositionColumns::Name),
            QHeaderView::Stretch
        );
    }

}   // namespace ui