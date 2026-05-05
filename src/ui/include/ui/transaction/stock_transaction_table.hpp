#ifndef __UI__INCLUDE__UI__TRANSACTION__STOCK_TRANSACTION_TABLE_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__STOCK_TRANSACTION_TABLE_HPP__

#include <qtablewidget.h>

#include "drafts/transaction_draft.hpp"
#include "ui/transaction/transaction_table.hpp"

namespace ui
{

    /**
     * @brief A draft representation of a stock transaction
     *
     */
    class StockTransactionTableModel : public TransactionTableModel
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

#endif   // __UI__INCLUDE__UI__TRANSACTION__STOCK_TRANSACTION_TABLE_HPP__