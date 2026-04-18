#ifndef __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__

#include <qwidget.h>

#include "drafts/account_draft.hpp"
#include "drafts/transaction_draft.hpp"

class QComboBox;     // Forward declaration
class QLabel;        // Forward declaration
class QLayout;       // Forward declaration
class QPushButton;   // Forward declaration
class QLineEdit;     // Forward declaration
class QFormLayout;   // Forward declaration

namespace ui
{
    class AmountLineEdit;   // Forward declaration

    /**
     * @brief Widget for creating or editing a transaction, with fields for
     * selecting accounts, entering amounts, and specifying transaction details
     *
     * This widget provides a form for creating or editing a transaction. It
     * includes fields for selecting the accounts involved in the transaction,
     * entering the amount, and specifying other details. The widget validates
     * the input and emits a signal with the transaction draft when the user
     * submits the form.
     */
    // TODO(97gamjak): move this one to a separate file
    class ICreateTransactionWidget : public QWidget
    {
        Q_OBJECT

       public:
        explicit ICreateTransactionWidget(QWidget* parent);

       signals:
        /**
         * @brief Signal emitted when a transaction draft is ready to be created
         *
         * @param draft
         */
        void createTransactionRequested(const drafts::TransactionDraft& draft);
    };

    /**
     * @brief Widget for creating or editing a transaction, with fields for
     * selecting accounts, entering amounts, and specifying transaction details
     *
     * This widget provides a form for creating or editing a transaction. It
     * includes fields for selecting the accounts involved in the transaction,
     * entering the amount, and specifying other details. The widget validates
     * the input and emits a signal with the transaction draft when the user
     * submits the form.
     */
    class EmptyTransactionWidget : public ICreateTransactionWidget
    {
       public:
        explicit EmptyTransactionWidget(QWidget* parent);
    };

    /**
     * @brief Widget for creating or editing a transaction, with fields for
     * selecting accounts, entering amounts, and specifying transaction details
     *
     * This widget provides a form for creating or editing a transaction. It
     * includes fields for selecting the accounts involved in the transaction,
     * entering the amount, and specifying other details. The widget validates
     * the input and emits a signal with the transaction draft when the user
     * submits the form.
     */
    class NonEmptyTransactionWidget : public ICreateTransactionWidget
    {
        Q_OBJECT
       private:
        /// A list of account drafts to populate the account selection field
        std::vector<drafts::AccountDraft> _accounts;

        /// The type of transaction this widget is for (e.g. Deposit,
        /// Withdrawal)
        TransactionType _type;

        /// The layout for the widget
        QComboBox* _accountsSelection = nullptr;
        /// The field for entering the transaction amount
        AmountLineEdit* _amountField = nullptr;
        /// A label for displaying the currency symbol next to the amount field
        QLabel* _currencyLabel = nullptr;
        /// The button for submitting the form to create the transaction draft
        QPushButton* _addButton = nullptr;
        /// The form layout for arranging the input fields and labels
        QFormLayout* _layout = nullptr;

       public:
        explicit NonEmptyTransactionWidget(
            QWidget*                          parent,
            TransactionType                   type,
            std::vector<drafts::AccountDraft> accounts
        );

        /**
         * @brief Get the Transaction Draft object
         *
         * @return drafts::TransactionDraft
         */
        [[nodiscard]]
        virtual drafts::TransactionDraft getDraft() const = 0;

       private:
        void _setAccounts();
        void _onAccountSelected(int index);
        void _emitOk();
        void _enableAddButtonIfValid();

       protected:
        [[nodiscard]]
        TransactionType _getTransactionType() const;

        [[nodiscard]]
        std::optional<drafts::AccountDraft> _getSelectedAccount() const;

        [[nodiscard]]
        micro_units _getAmount() const;
    };

    /**
     * @brief Factory function to create a transaction widget based on the
     * transaction type
     *
     * @param parent The parent widget
     * @param type The type of the transaction
     * @param accounts The list of account drafts to choose from
     *
     * @return ICreateTransactionWidget* A pointer to the created transaction
     * widget
     */
    class DepositWithdrawalWidget : public NonEmptyTransactionWidget
    {
        using NonEmptyTransactionWidget::NonEmptyTransactionWidget;

       public:
        [[nodiscard]] drafts::TransactionDraft getDraft() const override;
    };

    /**
     * @brief Factory function to create a transaction widget based on the
     * transaction type
     *
     * @param parent The parent widget
     * @param type The type of the transaction
     * @param accounts The list of account drafts to choose from
     *
     * @return ICreateTransactionWidget* A pointer to the created transaction
     * widget
     */
    ICreateTransactionWidget* makeTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
    );

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__