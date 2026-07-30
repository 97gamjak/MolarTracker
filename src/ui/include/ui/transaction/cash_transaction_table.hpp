#ifndef __UI__INCLUDE__UI__TRANSACTION__CASH_TRANSACTION_TABLE_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__CASH_TRANSACTION_TABLE_HPP__

#include <qabstractitemmodel.h>
#include <qtablewidget.h>

#include "common/container/id_map.hpp"
#include "config/id_types.hpp"

namespace drafts
{
    class CashTransactionOverview;   // Forward declaration
}   // namespace drafts

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
    class CashTransactionTableModel : public QAbstractTableModel
    {
        Q_OBJECT
       private:
        /// The list of transactions to display in the table
        std::vector<drafts::CashTransactionOverview> _transactions;

        /// A map of account IDs to account names for display purposes
        IdMap<AccountId, std::string> _accountIdToName;

       public:
        explicit CashTransactionTableModel(QObject* parent = nullptr);
        ~CashTransactionTableModel() override;

        [[nodiscard]] int columnCount(const QModelIndex& parent) const override;

        [[nodiscard]] static int getDescriptionIndex();
        [[nodiscard]] static int getDateIndex();

        void setTransactions(
            std::vector<drafts::CashTransactionOverview> transactions,
            IdMap<AccountId, std::string>                accountIdToName
        );

        [[nodiscard]]
        int rowCount(const QModelIndex& parent) const override;

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

       private:
        [[nodiscard]]
        QVariant _displayData(
            const drafts::CashTransactionOverview& transaction,
            int                                    col
        ) const;

        [[nodiscard]]
        static QVariant _decorationData(
            const drafts::CashTransactionOverview& transaction,
            int                                    col
        );

        [[nodiscard]]
        static QVariant _textAlignmentData(int col);

        [[nodiscard]]
        static QString _getColLabel(int col);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__CASH_TRANSACTION_TABLE_HPP__