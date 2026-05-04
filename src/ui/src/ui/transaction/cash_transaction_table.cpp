#include "ui/transaction/cash_transaction_table.hpp"

#include <qboxlayout.h>
#include <qheaderview.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qvariant.h>

#include <QColor>
#include <QDateTime>
#include <QFont>
#include <mstd/enum.hpp>

#include "drafts/transaction_draft.hpp"

namespace ui
{

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define COLUMN_LIST(X)  \
    X(Date)             \
    X(Type)             \
    X(Account)          \
    X(ReferenceAccount) \
    X(Amount)           \
    X(Description)

    MSTD_ENUM(CashTransactionColumn, std::uint8_t, COLUMN_LIST);

    namespace
    {
        QString getColLabel(CashTransactionColumn col)
        {
            if (col == CashTransactionColumn::ReferenceAccount)
                return "Reference Account";

            return QString::fromStdString(
                CashTransactionColumnMeta::toString(col)
            );
        }

        CashTransactionColumn getColFromIndex(int index)
        {
            return static_cast<CashTransactionColumn>(index);
        }
    }   // namespace

    /**
     * @brief get the number of columns in the table model
     *
     * @param parent
     * @return int
     */
    int CashTransactionTableModel::columnCount(const QModelIndex& parent) const
    {
        return parent.isValid() ? 0 : CashTransactionColumnMeta::size;
    }

    int CashTransactionTableModel::getDescriptionIndex() const
    {
        return static_cast<int>(CashTransactionColumn::Description);
    }

    int CashTransactionTableModel::getDateIndex() const
    {
        return static_cast<int>(CashTransactionColumn::Date);
    }

    /**
     * @brief display data for the table model
     *
     * @param transaction
     * @param col
     * @return QVariant
     */
    QVariant CashTransactionTableModel::_displayData(
        const drafts::TransactionOverviewDraft& transaction,
        int                                     col
    ) const
    {
        switch (getColFromIndex(col))
        {
            case CashTransactionColumn::Date:
            {
                return transaction.getTimestamp().toQDateTime().toString(
                    "yyyy-MM-dd"
                );
            }
            case CashTransactionColumn::Description:
                return QString::fromStdString(
                    transaction.getComment().value_or("")
                );
            case CashTransactionColumn::Type:
                return QString::fromStdString("");
            case CashTransactionColumn::Account:
            {
                if (!_getAccountIdToNameMap().contains(
                        transaction.getEntries().front().getAccountId()
                    ))
                    return "";

                return QString::fromStdString(
                    _getAccountIdToNameMap().at(
                        transaction.getEntries().front().getAccountId()
                    )
                );
            }
            case CashTransactionColumn::ReferenceAccount:
            {
                if (!_getAccountIdToNameMap().contains(
                        transaction.getEntries().back().getAccountId()
                    ))
                    return "";

                return QString::fromStdString(
                    _getAccountIdToNameMap().at(
                        transaction.getEntries().back().getAccountId()
                    )
                );
            }
            case CashTransactionColumn::Amount:
                return QString::fromStdString(
                    transaction.getEntries().front().getCash().toString(2)
                );
        }
        return {};
    }

    /**
     * @brief decoration data for the table model
     *
     * @param transaction
     * @param col
     * @return QVariant
     */
    QVariant CashTransactionTableModel::_decorationData(
        const drafts::TransactionOverviewDraft& transaction,
        int                                     col
    ) const
    {
        if (getColFromIndex(col) != CashTransactionColumn::Amount)
            return {};

        return transaction.getEntries().front().getCash().isPositive()
                   ? QColor(Qt::GlobalColor::green)
                   : QColor(Qt::GlobalColor::red);
    }
    /**
     * @brief text alignment data for the table model
     *
     * @param col
     * @return QVariant
     */
    QVariant CashTransactionTableModel::_textAlignmentData(int col) const
    {
        if (getColFromIndex(col) == CashTransactionColumn::Amount)
            return {Qt::AlignRight | Qt::AlignVCenter};
        return {Qt::AlignLeft | Qt::AlignVCenter};
    }

    QString CashTransactionTableModel::_getColLabel(int col) const
    {
        return getColLabel(static_cast<CashTransactionColumn>(col));
    }

}   // namespace ui