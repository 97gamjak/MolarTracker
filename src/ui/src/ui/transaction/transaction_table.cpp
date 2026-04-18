#include "ui/transaction/transaction_table.hpp"

#include <qboxlayout.h>
#include <qheaderview.h>
#include <qlineedit.h>
#include <qvariant.h>

#include <QColor>
#include <QDateTime>
#include <QFont>
#include <mstd/enum.hpp>

#include "config/id_types.hpp"
#include "finance/currency.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

#define COLUMN_LIST(X) \
    X(Date)            \
    X(Type)            \
    X(FromAccount)     \
    X(ToAccount)       \
    X(Amount)          \
    X(Description)

    MSTD_ENUM(Column, std::uint8_t, COLUMN_LIST);

    TransactionTableModel::TransactionTableModel(QObject* parent)
        : QAbstractTableModel(parent)
    {
    }

    void TransactionTableModel::setTransactions(
        std::vector<drafts::TransactionDraft> transactions,
        IdToNameMap                           accountIdToName
    )
    {
        beginResetModel();
        _transactions    = std::move(transactions);
        _accountIdToName = std::move(accountIdToName);
        endResetModel();
    }

    int TransactionTableModel::rowCount(const QModelIndex& parent) const
    {
        return parent.isValid() ? 0 : static_cast<int>(_transactions.size());
    }

    int TransactionTableModel::columnCount(const QModelIndex& parent) const
    {
        return parent.isValid() ? 0 : ColumnMeta::size;
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
                return transaction.timestamp.toQDateTime().toString(
                    "yyyy-MM-dd"
                );
            }
            case Column::Description:
                return QString::fromStdString(transaction.comment.value_or(""));
            case Column::Type:
                return QString::fromStdString("");
            case Column::FromAccount:
                return QString::fromStdString(
                    _accountIdToName.at(transaction.entries.front().accountId)
                );
            case Column::ToAccount:
                return QString::fromStdString(
                    _accountIdToName.at(transaction.entries.back().accountId)
                );
            case Column::Amount:
                return QString::fromStdString(
                    transaction.entries.front().cash.toString()
                );
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
        return transaction.entries.front().cash.getAmount() >= 0.0
                   ? QColor(59, 109, 17)
                   : QColor(163, 45, 45);
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

        return QString::fromStdString(
            ColumnMeta::toString(static_cast<Column>(section))
        );
    }

    Qt::ItemFlags TransactionTableModel::flags(const QModelIndex& index) const
    {
        if (!index.isValid())
            return Qt::NoItemFlags;
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    int TransactionTableModel::getDescriptionIndex()
    {
        return static_cast<int>(Column::Description);
    }

    int TransactionTableModel::getDateIndex()
    {
        return static_cast<int>(Column::Date);
    }

}   // namespace ui