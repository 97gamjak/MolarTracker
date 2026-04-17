#ifndef __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__

#include <qwidget.h>

#include "drafts/account_draft.hpp"

class QComboBox;   // Forward declaration
class QLabel;      // Forward declaration
class QLayout;     // Forward declaration

namespace ui
{
    class CreateTransactionWidget : public QWidget
    {
        Q_OBJECT

       private:
        std::vector<drafts::AccountDraft> _accounts;

        QComboBox* _accountsSelection = nullptr;
        QLabel*    _currencyLabel     = nullptr;

        QLayout* _layout = nullptr;

       public:
        explicit CreateTransactionWidget(
            QWidget*                          parent,
            std::vector<drafts::AccountDraft> accounts
        );

        [[nodiscard]]
        std::optional<TransactionType> getTransactionType() const;

       private:
        void _setAccounts();
        void _buildUI();
        void _onAccountSelected(int index);
    };

    class EmptyTransactionWidget : public CreateTransactionWidget
    {
       public:
        explicit EmptyTransactionWidget(QWidget* parent);
    };

    class DepositWithdrawalWidget : public CreateTransactionWidget
    {
        using CreateTransactionWidget::CreateTransactionWidget;
    };

    CreateTransactionWidget* makeTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
    );

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__