#ifndef __UI__INCLUDE__UI__TRANSACTION__CASH_TRANSACTION_TABLE_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__CASH_TRANSACTION_TABLE_HPP__

#include <qtablewidget.h>

#include "drafts/transaction_draft.hpp"
#include "ui/transaction/transaction_table.hpp"

namespace ui
{
    enum class Column : std::uint8_t;

    /**
     * @brief Table model for displaying cash transactions in a QTableView
     *
     * This model provides the data and behavior for displaying transactions in
     * a QTableView. It holds a list of TransactionDrafts and maps account IDs
     * to account names for display purposes. The model implements the necessary
     * methods for row and column count, data retrieval, header data, and item
     * flags to enable proper display and interaction with the transaction data.
     */
    class CashTransactionTableModel : public TransactionTableModel
    {
        Q_OBJECT

       public:
        using TransactionTableModel::TransactionTableModel;

        [[nodiscard]] int columnCount(const QModelIndex& parent) const override;

        [[nodiscard]] int getDescriptionIndex() const override;
        [[nodiscard]] int getDateIndex() const override;

       private:
        [[nodiscard]]
        QVariant _displayData(
            const drafts::TransactionOverviewDraft& transaction,
            int                                     col
        ) const override;

        [[nodiscard]]
        QVariant _decorationData(
            const drafts::TransactionOverviewDraft& transaction,
            int                                     col
        ) const override;

        [[nodiscard]]
        QVariant _textAlignmentData(int col) const override;

        [[nodiscard]]
        QString _getColLabel(int col) const override;
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__CASH_TRANSACTION_TABLE_HPP__