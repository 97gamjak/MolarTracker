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

    class ICreateTransactionWidget : public QWidget
    {
        Q_OBJECT

       public:
        explicit ICreateTransactionWidget(QWidget* parent);

       signals:
        void createTransactionRequested(const drafts::TransactionDraft& draft);
    };

    class EmptyTransactionWidget : public ICreateTransactionWidget
    {
       public:
        explicit EmptyTransactionWidget(QWidget* parent);
    };

    class NonEmptyTransactionWidget : public ICreateTransactionWidget
    {
        Q_OBJECT
       private:
        std::vector<drafts::AccountDraft> _accounts;
        TransactionType                   _type;

        QComboBox*      _accountsSelection = nullptr;
        AmountLineEdit* _amountField       = nullptr;
        QLabel*         _currencyLabel     = nullptr;
        QPushButton*    _addButton         = nullptr;

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
        void _buildUI();
        void _onAccountSelected(int index);
        void _emitOk();

       protected:
        [[nodiscard]]
        TransactionType _getTransactionType() const;

        [[nodiscard]]
        std::optional<drafts::AccountDraft> _getSelectedAccount() const;

        [[nodiscard]]
        micro_units _getAmount() const;
    };

    class DepositWithdrawalWidget : public NonEmptyTransactionWidget
    {
        using NonEmptyTransactionWidget::NonEmptyTransactionWidget;

       public:
        [[nodiscard]] drafts::TransactionDraft getDraft() const override;
    };

    ICreateTransactionWidget* makeTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
    );

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DETAILS_HPP__