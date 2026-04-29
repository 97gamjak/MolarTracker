// deposit_withdrawal_widget.hpp
#ifndef __UI__INCLUDE__UI__TRANSACTION__DEPOSIT_WITHDRAWAL_WIDGET_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__DEPOSIT_WITHDRAWAL_WIDGET_HPP__

#include <qwidget.h>

#include <vector>

#include "config/finance.hpp"
#include "drafts/account_draft.hpp"
#include "drafts/transaction_draft.hpp"
#include "ui/transaction/i_create_transaction_widget.hpp"

class QFormLayout;   // Forward declaration
class QLabel;        // Forward declaration
class QPushButton;   // Forward declaration

namespace ui
{

    class AccountCombo;   // Forward declaration
    class AmountRow;      // Forward declaration

    /**
     * @brief Widget for creating a deposit or withdrawal transaction
     *
     * Provides a form with an account selector, amount field, and currency
     * label. The add button is enabled only when both the account and a
     * non-zero valid amount are present.
     */
    class DepositWithdrawalWidget : public ICreateTransactionWidget
    {
        Q_OBJECT

       private:
        /// The type of transaction this widget is creating (Deposit or
        /// Withdrawal)
        TransactionType _type;

        /// The layout for the widget
        QFormLayout* _layout;

        /// The combo box for selecting the account to deposit to or withdraw
        /// from
        AccountCombo* _accountCombo;

        /// The row for entering the amount to deposit or withdraw
        AmountRow* _amountRow;

        /// The label for displaying the currency of the selected account
        QLabel* _currencyLabel;

        /// The button for adding the transaction
        QPushButton* _addButton;

       public:
        explicit DepositWithdrawalWidget(
            TransactionType                   type,
            std::vector<drafts::AccountDraft> accounts,
            QWidget*                          parent = nullptr
        );

        [[nodiscard]] TransactionType getTransactionType() const override;
        [[nodiscard]] drafts::CreateCashTransactionDraft getDraft() const;

       private:
        void _onAccountSelected(const drafts::AccountDraft& account);
        void _updateAddButton();
        void _emitOk();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__DEPOSIT_WITHDRAWAL_WIDGET_HPP__