#include "ui/transaction/transaction_table.hpp"

#include <qboxlayout.h>
#include <qheaderview.h>
#include <qlineedit.h>
#include <qvariant.h>

#include <QColor>
#include <QDateTime>
#include <QFont>

#include "config/id_types.hpp"
#include "finance/currency.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    namespace
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
                case TransactionTableModel::Column::FromAccount:
                    return "From Account";
                case TransactionTableModel::Column::ToAccount:
                    return "To Account";
                case TransactionTableModel::Column::Amount:
                    return "Amount";
                case TransactionTableModel::Column::Currency:
                    return "Currency";
                case TransactionTableModel::Column::Count:
                    break;
            }
            return {};
        }
    }   // namespace

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
                return QString::number(
                    transaction.entries.front().amount / 100,
                    'f',
                    2
                );
            case Column::Currency:
                return QString::fromStdString(
                    finance::getSymbol(transaction.entries.front().currency)
                );
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

}   // namespace ui