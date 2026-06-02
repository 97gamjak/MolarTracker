#include "ui/position/position_table_view.hpp"

#include <QHeaderView>
#include <QSortFilterProxyModel>

#include "ui/position/position_table_model.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

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

    void PositionTableView::setPositionModel(StockPositionTableModel* model)
    {
        _proxy = utils::makeQChild<QSortFilterProxyModel>(this);
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

    int PositionTableView::selectedRow() const
    {
        const auto indexes = selectionModel()->selectedRows();
        if (indexes.isEmpty())
            return -1;
        return _proxy->mapToSource(indexes.first()).row();
    }

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