#ifndef __UI__INCLUDE__UI__TRANSACTION__TRANSACTIONS_OVERVIEW_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__TRANSACTIONS_OVERVIEW_HPP__

#include <qwidget.h>

#include "drafts/transaction_draft.hpp"
#include "ui/transaction/transaction_table.hpp"

namespace ui
{
    class TransactionsOverview : public QWidget
    {
        Q_OBJECT

       private:
        TransactionTableModel* _model;
        QSortFilterProxyModel* _proxy;
        QTableView*            _table;

       public:
        explicit TransactionsOverview(QWidget* parent);

        void refresh(
            const std::vector<drafts::TransactionDraft>& transactions,
            const std::unordered_map<AccountId, std::string, AccountId::Hash>&
                accountIdToName
        );

       private:
        void _setupTable();
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__TRANSACTIONS_OVERVIEW_HPP__