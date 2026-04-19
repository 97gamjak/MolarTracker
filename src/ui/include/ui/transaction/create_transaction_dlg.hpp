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

        void reset();

       signals:
        /**
         * @brief signal emitted when a transaction type is changed
         *
         * @param type
         */
        void transactionTypeChanged(TransactionType type);

        /**
         * @brief signal emitted when a transaction draft is ready to be created
         *
         * @param draft The transaction draft containing the details of the
         * transaction to be created, this will be emitted when the user has
         * filled out the necessary information in the dialog and is ready to
         * create the transaction, allowing the controller to handle the
         * creation of the transaction based on the provided draft.
         */
        void createTransactionRequested(const drafts::TransactionDraft& draft);

       private:
        void _buildUI();
        void _onTransactionTypeChanged(int index);
        void _setWindowTitle(TransactionType type);
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DLG_HPP__
