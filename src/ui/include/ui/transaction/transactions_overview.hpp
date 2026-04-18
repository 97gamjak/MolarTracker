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
        TransactionTable* _transactionTable = nullptr;

       public:
        explicit TransactionsOverview(QWidget* parent);

        void refresh(const std::vector<drafts::TransactionDraft>& transactions);
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__TRANSACTIONS_OVERVIEW_HPP__