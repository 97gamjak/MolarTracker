#include "ui/side_bar/transaction_category.hpp"

#include <QMenu>

#include "config/finance.hpp"
#include "ui/side_bar/category.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{
    /**
     * @brief Construct a new Transaction Category:: Transaction Category object
     *
     */
    TransactionCategory::TransactionCategory()
        : Category("Transactions", SideBarItemType::TransactionCategory),
          _createDepositAction(nullptr),
          _createWithdrawalAction(nullptr),
          _createStockTransactionAction(nullptr)
    {
    }

    /**
     * @brief Get the create deposit action for the transaction category
     *
     * @return QAction* The create deposit action for the transaction category
     */
    QAction* TransactionCategory::getCreateDepositAction() const
    {
        return _createDepositAction;
    }

    /**
     * @brief Get the create withdrawal action for the transaction category
     *
     * @return QAction* The create withdrawal action for the transaction
     * category
     */
    QAction* TransactionCategory::getCreateWithdrawalAction() const
    {
        return _createWithdrawalAction;
    }

    /**
     * @brief Get the create stock transaction action for the transaction
     * category
     *
     * @return QAction* The create stock transaction action for the transaction
     * category
     */
    QAction* TransactionCategory::getCreateStockTransactionAction() const
    {
        return _createStockTransactionAction;
    }

    /**
     * @brief Populate the context menu for the transaction category
     *
     * @param menu The context menu to populate
     */
    void TransactionCategory::populateContextMenu(QMenu& menu)
    {
        using enum TransactionType;

        const auto depositName = "Create " + utils::toQString(Deposit);
        _createDepositAction   = menu.addAction(depositName);
        _createDepositAction->setData(QVariant::fromValue(Deposit));

        const auto withdrawalName = "Create " + utils::toQString(Withdrawal);
        _createWithdrawalAction   = menu.addAction(withdrawalName);
        _createWithdrawalAction->setData(QVariant::fromValue(Withdrawal));

        const auto stockTransactionName =
            "Create " + utils::toQString(TransactionType::Stock) +
            " Transaction";
        _createStockTransactionAction = menu.addAction(stockTransactionName);
        _createStockTransactionAction->setData(
            QVariant::fromValue(TransactionType::Stock)
        );
    }

}   // namespace ui