#ifndef __UI__INCLUDE__UI__TRANSACTION__TRANSACTIONS_OVERVIEW_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__TRANSACTIONS_OVERVIEW_HPP__

#include <qsortfilterproxymodel.h>
#include <qwidget.h>

#include "ui/transaction/transaction_table.hpp"

class QSortFilterProxyModel;   // Forward declaration
class QTableView;              // Forward declaration

namespace ui
{
    class CashTransactionTableModel;    // Forward declaration
    class StockTransactionTableModel;   // Forward declaration
    class TransactionTableModel;        // Forward declaration

    /**
     * @brief Widget for displaying an overview of transactions in a table view
     *
     * This widget provides a table view for displaying an overview of
     * transactions. It uses a TransactionTableModel to manage the data and a
     * QSortFilterProxyModel to enable sorting and filtering of the
     * transactions. The widget provides a method to refresh the displayed
     * transactions based on a list of TransactionDrafts and a mapping of
     * account IDs to account names for display purposes.
     */
    class TransactionsOverview : public QWidget
    {
        Q_OBJECT

       private:
        /// The model for the transaction table
        CashTransactionTableModel* _cashModel;
        /// The model for the stock transaction table
        StockTransactionTableModel* _stockModel;
        /// The proxy model for sorting and filtering the transaction table
        QSortFilterProxyModel* _cashProxy;
        /// The proxy model for sorting and filtering the stock transaction
        /// table
        QSortFilterProxyModel* _stockProxy;
        /// The table view for displaying the transactions
        QTableView* _cashTable;
        /// The table view for displaying the stock transactions
        QTableView* _stockTable;

       public:
        explicit TransactionsOverview(QWidget* parent);

        void refresh(
            const std::vector<drafts::TransactionOverviewDraft>&
                cashTransactions,
            const std::vector<drafts::TransactionOverviewDraft>&
                stockTransactions,
            const std::unordered_map<AccountId, std::string, AccountId::Hash>&
                accountIdToName
        );

       private:
        static void _setupTable(
            QTableView*            table,
            QSortFilterProxyModel* proxy,
            TransactionTableModel* model
        );
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__TRANSACTIONS_OVERVIEW_HPP__