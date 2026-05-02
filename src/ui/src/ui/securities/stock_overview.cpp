#include "ui/securities/stock_overview.hpp"

#include <qboxlayout.h>
#include <qheaderview.h>
#include <qsortfilterproxymodel.h>
#include <qt6/QtWidgets/qlineedit.h>
#include <qtableview.h>

#include "ui/securities/stock_info_model.hpp"
#include "utils/qt_helpers.hpp"

using utils::makeQChild;

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

        emit tickerSelected(ticker);
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

}   // namespace ui