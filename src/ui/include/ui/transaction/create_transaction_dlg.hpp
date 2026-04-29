#ifndef __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DLG_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DLG_HPP__

#include <qstackedwidget.h>
#include <qvariant.h>

#include <QComboBox>

#include "config/finance.hpp"
#include "drafts/account_draft.hpp"
#include "drafts/transaction_draft.hpp"
#include "i_create_transaction_widget.hpp"
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
        /// A combo box for selecting the transaction type
        QComboBox* _transactionType = nullptr;

        /// A map of transaction types to their corresponding detail widgets
        std::unordered_map<TransactionType, ICreateTransactionWidget*>
            _widgetMap;

        /// A stacked widget for displaying the transaction detail widgets based
        /// on the selected transaction type
        QStackedWidget* _stack = nullptr;

       public:
        explicit CreateTransactionDialog(QWidget* parent);

        void setWidget(
            TransactionType                   type,
            std::vector<drafts::AccountDraft> accounts
        );

        void setWidget(
            TransactionType                   type,
            std::vector<drafts::AccountDraft> accounts,
            std::vector<drafts::AccountDraft> referenceAccounts
        );

        void reset();

       signals:
        /**
         * @brief signal emitted when a transaction type is changed
         *
         * @param type The new transaction type
         */
        void transactionTypeChanged(TransactionType type);

        void createCashTransactionRequested(
            drafts::CreateCashTransactionDraft draft
        );

       private:
        void _buildUI();
        void _onTransactionTypeChanged(int index);
        void _setWindowTitle(TransactionType type);
        void _onCreateTransactionRequested(TransactionType type);
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DLG_HPP__
