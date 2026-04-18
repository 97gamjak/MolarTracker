#ifndef __UI__INCLUDE__UI__TRANSACTION__TRANSACTION_TABLE_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__TRANSACTION_TABLE_HPP__

#include <qtablewidget.h>

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

#include "drafts/transaction_draft.hpp"

namespace ui
{
    enum class Column : std::uint8_t;

    /**
     * @brief Table model for displaying transactions in a QTableView
     *
     * This model provides the data and behavior for displaying transactions in
     * a QTableView. It holds a list of TransactionDrafts and maps account IDs
     * to account names for display purposes. The model implements the necessary
     * methods for row and column count, data retrieval, header data, and item
     * flags to enable proper display and interaction with the transaction data.
     */
    class TransactionTableModel : public QAbstractTableModel
    {
        Q_OBJECT

       private:
        /// alias for the map of account IDs to account names
        using IdToNameMap =
            std::unordered_map<AccountId, std::string, AccountId::Hash>;

        /// The list of transactions to display in the table
        std::vector<drafts::TransactionDraft> _transactions;

        /// A map of account IDs to account names for display purposes
        IdToNameMap _accountIdToName;

       public:
        explicit TransactionTableModel(QObject* parent);

        void setTransactions(
            std::vector<drafts::TransactionDraft> transactions,
            IdToNameMap                           accountIdToName
        );

        [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
        [[nodiscard]] int columnCount(const QModelIndex& parent) const override;
        [[nodiscard]]
        QVariant data(const QModelIndex& index, int role) const override;
        [[nodiscard]]
        QVariant headerData(
            int             section,
            Qt::Orientation orientation,
            int             role
        ) const override;
        [[nodiscard]]
        Qt::ItemFlags flags(const QModelIndex& index) const override;

        [[nodiscard]] static int getDescriptionIndex();
        [[nodiscard]] static int getDateIndex();

       private:
        [[nodiscard]]
        QVariant _displayData(
            const drafts::TransactionDraft& transaction,
            Column                          col
        ) const;
        [[nodiscard]]
        QVariant _decorationData(
            const drafts::TransactionDraft& transaction,
            Column                          col
        ) const;
        [[nodiscard]]
        QVariant _textAlignmentData(Column col) const;
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__TRANSACTION_TABLE_HPP__