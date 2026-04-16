#include "ui/side_bar/transaction_category.hpp"

#include <QMenu>

#include "config/finance.hpp"
#include "ui/side_bar/category.hpp"

namespace ui
{
    /**
     * @brief Construct a new Transaction Category:: Transaction Category object
     *
     */
    TransactionCategory::TransactionCategory()
        : Category("Transactions", SideBarItemType::TransactionCategory),
          _createAction(nullptr),
          _createDepositAction(nullptr),
          _createWithdrawalAction(nullptr)
    {
    }

    /**
     * @brief Get the create action for the transaction category
     *
     * @return QAction* The create action for the transaction category
     */
    QAction* TransactionCategory::getCreateAction() const
    {
        return _createAction;
    }

    QAction* TransactionCategory::getCreateDepositAction() const
    {
        return _createDepositAction;
    }

    QAction* TransactionCategory::getCreateWithdrawalAction() const
    {
        return _createWithdrawalAction;
    }

    /**
     * @brief Populate the context menu for the transaction category
     *
     * @param menu The context menu to populate
     */
    void TransactionCategory::populateContextMenu(QMenu& menu)
    {
        _createAction = menu.addAction("Create Transaction");

        _createDepositAction = menu.addAction(
            "Create " +
            QString::fromStdString(
                TransactionTypeMeta::toString(TransactionType::Deposit)
            )
        );
        _createDepositAction->setData(
            QVariant::fromValue(TransactionType::Deposit)
        );

        _createWithdrawalAction = menu.addAction(
            "Create " +
            QString::fromStdString(
                TransactionTypeMeta::toString(TransactionType::Withdrawal)
            )
        );
        _createWithdrawalAction->setData(
            QVariant::fromValue(TransactionType::Withdrawal)
        );
    }

}   // namespace ui