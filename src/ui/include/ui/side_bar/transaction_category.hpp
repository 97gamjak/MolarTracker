#ifndef __UI__INCLUDE__UI__SIDE_BAR__TRANSACTION_CATEGORY_HPP__
#define __UI__INCLUDE__UI__SIDE_BAR__TRANSACTION_CATEGORY_HPP__

#include "category.hpp"

class QAction;   // Forward declaration

namespace ui
{
    /**
     * @brief Represents the transaction category in the UI
     *
     */
    class TransactionCategory : public Category
    {
       private:
        /// The create action for the transaction category
        QAction* _createAction;
        /// The create deposit action for the transaction category
        QAction* _createDepositAction;
        /// The create withdrawal action for the transaction category
        QAction* _createWithdrawalAction;

       public:
        explicit TransactionCategory();

        [[nodiscard]] QAction* getCreateAction() const;
        [[nodiscard]] QAction* getCreateDepositAction() const;
        [[nodiscard]] QAction* getCreateWithdrawalAction() const;

        void populateContextMenu(QMenu& menu) override;
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__SIDE_BAR__TRANSACTION_CATEGORY_HPP__