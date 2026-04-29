// make_transaction_widget.hpp
#ifndef __UI__INCLUDE__UI__TRANSACTION__MAKE_TRANSACTION_WIDGET_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__MAKE_TRANSACTION_WIDGET_HPP__

#include <vector>

#include "config/finance.hpp"
#include "drafts/account_draft.hpp"

class QWidget;   // Forward declaration

namespace ui
{

    class ICreateTransactionWidget;   // Forward declaration

    /**
     * @brief Factory function to create a transaction widget based on the
     * transaction type
     *
     * @param parent The parent widget
     * @param type The type of the transaction
     * @param accounts The list of account drafts to choose from
     *
     * @return ICreateTransactionWidget* A pointer to the created widget
     */
    [[nodiscard]] ICreateTransactionWidget* makeTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
    );

    /**
     * @brief Factory function to create a transaction widget based on the
     * transaction type, with an optional list of reference accounts for
     * security transaction types
     *
     * @param parent The parent widget
     * @param type The type of the transaction
     * @param accounts The list of account drafts to choose from
     * @param referenceAccounts The list of reference account drafts
     *
     * @return ICreateTransactionWidget* A pointer to the created widget
     */
    [[nodiscard]] ICreateTransactionWidget* makeTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts,
        std::vector<drafts::AccountDraft> referenceAccounts
    );

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__MAKE_TRANSACTION_WIDGET_HPP__