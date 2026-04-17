#ifndef __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DLG_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DLG_HPP__

#include <QComboBox>

#include "config/finance.hpp"
#include "drafts/account_draft.hpp"
#include "ui/base/dialog.hpp"

namespace ui
{
    class CreateTransactionWidget : public QWidget
    {
        Q_OBJECT

       public:
        explicit CreateTransactionWidget(QWidget* parent);
    };

    class EmptyTransactionWidget : public CreateTransactionWidget
    {
        using CreateTransactionWidget::CreateTransactionWidget;
    };

    class DepositWithdrawalWidget : public CreateTransactionWidget
    {
        using CreateTransactionWidget::CreateTransactionWidget;
    };

    /**
     * @brief Dialog for creating a new transaction
     *
     */
    class CreateTransactionDialog : public Dialog
    {
        Q_OBJECT

        std::vector<drafts::AccountDraft> _accounts;

       private:
        QComboBox* _transactionType = nullptr;

        CreateTransactionWidget* _createTransactionWidget = nullptr;

       public:
        explicit CreateTransactionDialog(
            QWidget*                          parent,
            std::vector<drafts::AccountDraft> accounts
        );
        explicit CreateTransactionDialog(
            QWidget*                          parent,
            std::vector<drafts::AccountDraft> accounts,
            TransactionType                   type
        );

       private:
        void _buildUI(std::optional<TransactionType> type);
        void _onTransactionTypeChanged(int index);
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DLG_HPP__
