#include "ui/transaction/transaction_table.hpp"

#include <qboxlayout.h>
#include <qheaderview.h>
#include <qlineedit.h>
#include <qvariant.h>

#include <QColor>
#include <QDateTime>
#include <QFont>

#include "utils/qt_helpers.hpp"

namespace ui
{

    constexpr int colCount =
        static_cast<int>(TransactionTableModel::Column::Count);

    QString columnLabel(TransactionTableModel::Column col)
    {
        switch (col)
        {
            case TransactionTableModel::Column::Date:
                return "Date";
            case TransactionTableModel::Column::Description:
                return "Description";
            case TransactionTableModel::Column::Type:
                return "Type";
            case TransactionTableModel::Column::Account:
                return "Account";
            case TransactionTableModel::Column::Amount:
                return "Amount";
            case TransactionTableModel::Column::Currency:
                return "Currency";
            case TransactionTableModel::Column::Count:
                break;
        }
        return {};
    }

    TransactionTableModel::TransactionTableModel(QObject* parent)
        : QAbstractTableModel(parent)
    {
    }

    void TransactionTableModel::setTransactions(
        std::vector<drafts::TransactionDraft> transactions
    )
    {
        beginResetModel();
        _transactions = std::move(transactions);
        endResetModel();
    }

    int TransactionTableModel::rowCount(const QModelIndex& parent) const
    {
        return parent.isValid() ? 0 : static_cast<int>(_transactions.size());
    }

    int TransactionTableModel::columnCount(const QModelIndex& parent) const
    {
        return parent.isValid() ? 0 : colCount;
    }

    QVariant TransactionTableModel::data(
        const QModelIndex& index,
        int                role
    ) const
    {
        if (!index.isValid() || index.row() >= rowCount({}))
            return {};
        const auto& transaction =
            _transactions[static_cast<std::size_t>(index.row())];
        const auto col = static_cast<Column>(index.column());

        switch (role)
        {
            case Qt::DisplayRole:
                return _displayData(transaction, col);
            case Qt::DecorationRole:
                return _decorationData(transaction, col);
            case Qt::TextAlignmentRole:
                return _textAlignmentData(col);
            default:
                return {};
        }
    }

    QVariant TransactionTableModel::_displayData(
        const drafts::TransactionDraft& transaction,
        Column                          col
    ) const
    {
        switch (col)
        {
            case Column::Date:
            {
                return QDateTime::fromMSecsSinceEpoch(
                           transaction.timestamp.toInt64(),
                           Qt::UTC
                )
                    .toLocalTime()
                    .toString("yyyy-MM-dd");
            }
            case Column::Description:
                return QString::fromStdString("");
            case Column::Type:
                return QString::fromStdString("");
            case Column::Account:
                return QString::fromStdString("");
            case Column::Amount:
                return QString::number(0.0, 'f', 2);
            case Column::Currency:
                return QString::fromStdString("");
            case Column::Count:
                break;
        }
        return {};
    }

    QVariant TransactionTableModel::_decorationData(
        const drafts::TransactionDraft& transaction,
        Column                          col
    ) const
    {
        if (col != Column::Amount)
            return {};
        // tint positive green, negative red — purely cosmetic
        return 100 >= 0.0 ? QColor(59, 109, 17) : QColor(163, 45, 45);
    }

    QVariant TransactionTableModel::_textAlignmentData(Column col) const
    {
        if (col == Column::Amount)
            return {Qt::AlignRight | Qt::AlignVCenter};
        return {Qt::AlignLeft | Qt::AlignVCenter};
    }

    QVariant TransactionTableModel::headerData(
        int             section,
        Qt::Orientation orientation,
        int             role
    ) const
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return {};
        return columnLabel(static_cast<Column>(section));
    }

    Qt::ItemFlags TransactionTableModel::flags(const QModelIndex& index) const
    {
        if (!index.isValid())
            return Qt::NoItemFlags;
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    TransactionTable::TransactionTable(QWidget* parent)
        : QWidget(parent),
          _model(utils::makeQChild<TransactionTableModel>(this)),
          _proxy(utils::makeQChild<QSortFilterProxyModel>(this)),
          _table(utils::makeQChild<QTableView>(this))
    {
        _proxy->setSourceModel(_model);
        _proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        _proxy->setFilterKeyColumn(-1);   // search all columns

        auto* search = utils::makeQChild<QLineEdit>(this);
        search->setPlaceholderText("Search transactions…");

        connect(
            search,
            &QLineEdit::textChanged,
            _proxy,
            &QSortFilterProxyModel::setFilterFixedString
        );

        _setupTable();

        auto* layout = new QVBoxLayout(this);
        layout->addWidget(search);
        layout->addWidget(_table);
    }

    void TransactionTable::_setupTable()
    {
        _table->setModel(_proxy);
        _table->setSortingEnabled(true);
        _table->setSelectionBehavior(QAbstractItemView::SelectRows);
        _table->setSelectionMode(QAbstractItemView::SingleSelection);
        _table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        _table->setAlternatingRowColors(true);
        _table->verticalHeader()->hide();

        auto* hdr = _table->horizontalHeader();
        hdr->setStretchLastSection(false);
        hdr->setSectionResizeMode(
            static_cast<int>(TransactionTableModel::Column::Description),
            QHeaderView::Stretch
        );   // description takes remaining space
        hdr->setSortIndicatorShown(true);

        // sensible default sort: newest first
        _table->sortByColumn(
            static_cast<int>(TransactionTableModel::Column::Date),
            Qt::DescendingOrder
        );
    }

    TransactionTableModel* TransactionTable::getModel() { return _model; }

}   // namespace ui