#ifndef __UI__INCLUDE__UI__TRANSACTION__TRANSACTIONS_OVERVIEW_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__TRANSACTIONS_OVERVIEW_HPP__

#include <qwidget.h>

#include "drafts/transaction_draft.hpp"
#include "ui/transaction/transaction_table.hpp"

namespace ui
{
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
        TransactionTableModel* _model;
        /// The proxy model for sorting and filtering the transaction table
        QSortFilterProxyModel* _proxy;
        /// The table view for displaying the transactions
        QTableView* _table;

       public:
        explicit TransactionsOverview(QWidget* parent);

        void refresh(
            const std::vector<drafts::TransactionOverviewDraft>& transactions,
            const std::unordered_map<AccountId, std::string, AccountId::Hash>&
                accountIdToName
        );

       private:
        void _setupTable();
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__TRANSACTIONS_OVERVIEW_HPP__