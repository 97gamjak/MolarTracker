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

        TransactionType _type;
        QFormLayout*    _layout;
        AccountCombo*   _accountCombo;
        AmountRow*      _amountRow;
        QLabel*         _currencyLabel;
        QPushButton*    _addButton;
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__DEPOSIT_WITHDRAWAL_WIDGET_HPP__