#ifndef __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__

#include <qvariant.h>
#include <qwidget.h>

#include "config/finance.hpp"
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

        [[nodiscard]]
        virtual TransactionType getTransactionType() const = 0;

       signals:
        /**
         * @brief Signal emitted when a transaction draft is ready to be
         * created
         *
         * @param type The type of the transaction (e.g., Deposit, Withdrawal)
         */
        void createTransactionRequested(TransactionType type);
    };

    class CreateTransactionWidget : public ICreateTransactionWidget
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
        explicit CreateTransactionWidget(
            QWidget*                          parent,
            TransactionType                   type,
            std::vector<drafts::AccountDraft> accounts
        );

        [[nodiscard]]
        TransactionType getTransactionType() const override;

       private:
        void         _setAccounts();
        void         _onAccountSelected(int index);
        virtual void _emitOk() = 0;
        void         _enableAddButtonIfValid();

       protected:
        [[nodiscard]]
        std::optional<drafts::AccountDraft> _getSelectedAccount() const;

        [[nodiscard]]
        AmountLineEdit* _getAmountField() const;
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
    class DepositWithdrawalWidget : public CreateTransactionWidget
    {
        using CreateTransactionWidget::CreateTransactionWidget;

       public:
        [[nodiscard]] drafts::CreateCashTransactionDraft getDraft() const;

        void _emitOk() override;
    };

    /**
     * @brief Widget for creating or editing a security transaction
     *
     * This widget provides a form for creating or editing a security
     * transaction. It includes fields for selecting the accounts involved in
     * the transaction, entering the amount, and specifying other details.
     * The widget validates the input and emits a signal with the transaction
     * draft when the user submits the form.
     */
    class SecurityWidget : public CreateTransactionWidget
    {
       private:
        /// A list of reference account drafts to populate the reference account
        /// selection field
        std::vector<drafts::AccountDraft> _referenceAccounts;

       public:
        explicit SecurityWidget(
            QWidget*                          parent,
            TransactionType                   type,
            std::vector<drafts::AccountDraft> accounts,
            std::vector<drafts::AccountDraft> referenceAccounts
        );
    };

    /**
     * @brief Widget for creating or editing a stock transaction
     *
     * This widget provides a form for creating or editing a stock
     * transaction. It includes fields for selecting the accounts involved in
     * the transaction, entering the amount, and specifying other details.
     * The widget validates the input and emits a signal with the transaction
     * draft when the user submits the form.
     */
    class StockWidget : public SecurityWidget
    {
       private:
        using SecurityWidget::SecurityWidget;

       private:
        void _emitOk() override
        {
            throw std::logic_error(
                "Stock transactions are not yet implemented"
            );
        }
    };

    ICreateTransactionWidget* makeTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
    );

    ICreateTransactionWidget* makeTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts,
        std::vector<drafts::AccountDraft> referenceAccounts
    );

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__