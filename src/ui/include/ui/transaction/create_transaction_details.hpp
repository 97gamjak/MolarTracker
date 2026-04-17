#ifndef __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__

#include <qwidget.h>

#include "drafts/account_draft.hpp"

namespace ui
{
    class CreateTransactionWidget : public QWidget
    {
        Q_OBJECT

       private:
        std::vector<drafts::AccountDraft> _accounts;

        std::optional<TransactionType> _transactionType;

       public:
        explicit CreateTransactionWidget(QWidget* parent);

        void setAccounts(std::vector<drafts::AccountDraft> accounts);

        [[nodiscard]]
        std::optional<TransactionType> getTransactionType() const;
    };

    class EmptyTransactionWidget : public CreateTransactionWidget
    {
        using CreateTransactionWidget::CreateTransactionWidget;
    };

    class DepositWithdrawalWidget : public CreateTransactionWidget
    {
        using CreateTransactionWidget::CreateTransactionWidget;
    };

    CreateTransactionWidget* makeTransactionWidget(
        QWidget*                       parent,
        std::optional<TransactionType> type
    );

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__