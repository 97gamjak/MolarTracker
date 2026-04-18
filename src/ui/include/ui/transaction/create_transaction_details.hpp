#ifndef __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__

#include <qwidget.h>

#include "drafts/account_draft.hpp"

class QComboBox;     // Forward declaration
class QLabel;        // Forward declaration
class QLayout;       // Forward declaration
class QLineEdit;     // Forward declaration
class QFormLayout;   // Forward declaration

namespace ui
{
    class AmountLineEdit;   // Forward declaration

    class ICreateTransactionWidget : public QWidget
    {
        Q_OBJECT

       public:
        explicit ICreateTransactionWidget(QWidget* parent);
    };

    class EmptyTransactionWidget : public ICreateTransactionWidget
    {
       public:
        explicit EmptyTransactionWidget(QWidget* parent);
    };

    class NonEmptyTransactionWidget : public ICreateTransactionWidget
    {
       private:
        std::vector<drafts::AccountDraft> _accounts;

        QComboBox*      _accountsSelection = nullptr;
        AmountLineEdit* _amountField       = nullptr;
        QLabel*         _currencyLabel     = nullptr;

        QFormLayout* _layout = nullptr;

       public:
        explicit NonEmptyTransactionWidget(
            QWidget*                          parent,
            std::vector<drafts::AccountDraft> accounts
        );

       private:
        void _setAccounts();
        void _buildUI();
        void _onAccountSelected(int index);
    };

    class DepositWithdrawalWidget : public NonEmptyTransactionWidget
    {
        using NonEmptyTransactionWidget::NonEmptyTransactionWidget;
    };

    ICreateTransactionWidget* makeTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
    );

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__