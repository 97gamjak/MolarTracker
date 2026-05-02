#include "ui/securities/stock_overview.hpp"

#include <qboxlayout.h>
#include <qheaderview.h>
#include <qsortfilterproxymodel.h>
#include <qt6/QtWidgets/qlineedit.h>
#include <qtableview.h>

#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{
    StockOverviewWidget::StockOverviewWidget(QWidget* parent)
        : QWidget(parent),
          _model(makeQChild<StockInfoTableModel>(this)),
          _proxy(makeQChild<QSortFilterProxyModel>(this)),
          _table(makeQChild<QTableView>(this))
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

        auto* topBar = new QHBoxLayout();
        topBar->addWidget(search);
        topBar->addStretch();

        auto* layout = new QVBoxLayout(this);
        layout->addLayout(topBar);
        layout->addWidget(_table);
    }

    void StockOverviewWidget::_setupTable()
    {
        _table->setModel(_proxy);
        _table->setSortingEnabled(true);
        _table->setSelectionBehavior(QAbstractItemView::SelectRows);
        _table->setSelectionMode(QAbstractItemView::SingleSelection);
        _table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        _table->setAlternatingRowColors(true);
        _table->verticalHeader()->hide();

        auto* hdr = _table->horizontalHeader();
        hdr->setSortIndicatorShown(true);
        hdr->setStretchLastSection(false);
        // Name column stretches; everything else is content-sized
        hdr->setSectionResizeMode(
            static_cast<int>(StockInfoTableModel::Column::Name),
            QHeaderView::Stretch
        );
        hdr->setSectionResizeMode(
            static_cast<int>(StockInfoTableModel::Column::Ticker),
            QHeaderView::ResizeToContents
        );
        hdr->setSectionResizeMode(
            static_cast<int>(StockInfoTableModel::Column::Currency),
            QHeaderView::ResizeToContents
        );
        hdr->setSectionResizeMode(
            static_cast<int>(StockInfoTableModel::Column::LastUpdated),
            QHeaderView::ResizeToContents
        );

        // Default sort: alphabetical by ticker
        _table->sortByColumn(
            static_cast<int>(StockInfoTableModel::Column::Ticker),
            Qt::AscendingOrder
        );

        connect(
            _table->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &StockOverviewWidget::_onSelectionChanged
        );
    }

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
                        static_cast<int>(StockInfoTableModel::Column::Ticker)
                    ),
                    Qt::DisplayRole
                )
                .toString();

        emit tickerSelected(ticker);
    }

}   // namespace ui