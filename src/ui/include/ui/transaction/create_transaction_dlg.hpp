#ifndef __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DLG_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DLG_HPP__

#include <QComboBox>
#include <optional>

#include "config/finance.hpp"
#include "drafts/account_draft.hpp"
#include "ui/base/dialog.hpp"

namespace ui
{

    class CreateTransactionWidget;

    /**
     * @brief Dialog for creating a new transaction
     *
     */
    class CreateTransactionDialog : public Dialog
    {
        Q_OBJECT

       private:
        QComboBox* _transactionType = nullptr;

        CreateTransactionWidget* _detailsWidget = nullptr;

       public:
        explicit CreateTransactionDialog(QWidget* parent);
        void setTransactionType(std::optional<TransactionType> type);
        void setWidget(
            TransactionType                   type,
            std::vector<drafts::AccountDraft> accounts
        );

       signals:
        void transactionTypeChanged(TransactionType type);

       private:
        void _buildUI();
        void _onTransactionTypeChanged(int index);
        void _setWindowTitle(std::optional<TransactionType> type);
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__CREATE_TRANSACTION_DLG_HPP__
