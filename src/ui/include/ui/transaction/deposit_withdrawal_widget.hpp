// deposit_withdrawal_widget.hpp
#ifndef __UI__INCLUDE__UI__TRANSACTION__DEPOSIT_WITHDRAWAL_WIDGET_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__DEPOSIT_WITHDRAWAL_WIDGET_HPP__

#include <qwidget.h>

#include <vector>

#include "common/finance.hpp"
#include "drafts/transaction/transaction_create_draft.hpp"
#include "ui/base/dialog.hpp"

class QFormLayout;   // Forward declaration
class QLabel;        // Forward declaration
class QPushButton;   // Forward declaration

namespace drafts
{
    class AccountDraft;   // Forward declaration
}   // namespace drafts

namespace ui
{

    class AccountCombo;     // Forward declaration
    class AmountRow;        // Forward declaration
    class CommentField;     // Forward declaration
    class TimestampField;   // Forward declaration

    /**
     * @brief Widget for creating a deposit or withdrawal transaction
     *
     * Provides a form with an account selector, amount field, and currency
     * label. The add button is enabled only when both the account and a
     * non-zero valid amount are present.
     */
    class DepositWithdrawalWidget : public Dialog
    {
        Q_OBJECT

       private:
        /// The type of transaction this widget is creating (Deposit or
        /// Withdrawal)
        TransactionType _type;

        /// The layout for the widget
        QFormLayout* _layout;

        /// The button for adding the transaction
        QPushButton* _addButton;

        struct Fields;
        /// The fields for the deposit/withdrawal widget
        std::unique_ptr<Fields> _fields;

       public:
        explicit DepositWithdrawalWidget(
            TransactionType                          type,
            const std::vector<drafts::AccountDraft>& accounts,
            QWidget*                                 parent = nullptr
        );

        ~DepositWithdrawalWidget() override;

        [[nodiscard]] TransactionType getTransactionType() const;
        void                          setTransactionType(TransactionType type);

        void updateAccounts(std::vector<drafts::AccountDraft> accounts);
        void refresh();

       signals:
        /**
         * @brief Emitted when a cash transaction is requested
         *
         * @param draft The draft of the cash transaction
         */
        void createCashTransactionRequested(
            drafts::CreateCashTransactionDraft draft
        );

       private:
        void _onAccountSelected(const drafts::AccountDraft& account);
        void _updateAddButton();
        void _emitOk();

        [[nodiscard]] drafts::CreateCashTransactionDraft _getDraft() const;
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__DEPOSIT_WITHDRAWAL_WIDGET_HPP__