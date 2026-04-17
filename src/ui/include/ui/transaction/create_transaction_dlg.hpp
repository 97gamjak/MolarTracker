#ifndef __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DLG_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DLG_HPP__

#include <qstackedwidget.h>

#include <QComboBox>

#include "config/finance.hpp"
#include "create_transaction_details.hpp"
#include "drafts/account_draft.hpp"
#include "ui/base/dialog.hpp"

namespace ui
{

    /**
     * @brief Dialog for creating a new transaction
     *
     */
    class CreateTransactionDialog : public Dialog
    {
        Q_OBJECT

       private:
        QComboBox* _transactionType = nullptr;

        std::unordered_map<TransactionType, CreateTransactionWidget*>
            _widgetMap;

        QStackedWidget* _stack = nullptr;

       public:
        explicit CreateTransactionDialog(QWidget* parent);
        void setWidget(
            TransactionType                   type,
            std::vector<drafts::AccountDraft> accounts
        );

       signals:
        void transactionTypeChanged(TransactionType type);

       private:
        void _buildUI();
        void _onTransactionTypeChanged(int index);
        void _setWindowTitle(TransactionType type);
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DLG_HPP__
