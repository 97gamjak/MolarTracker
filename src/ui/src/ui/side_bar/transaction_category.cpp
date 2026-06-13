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
          _createStockTransactionAction(nullptr),
          _createOptionTransactionAction(nullptr)
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
     * @brief Get the create option transaction action for the transaction
     * category
     *
     * @return QAction* The create option transaction action for the transaction
     * category
     */
    QAction* TransactionCategory::getCreateOptionTransactionAction() const
    {
        return _createOptionTransactionAction;
    }

    /**
     * @brief Populate the context menu for the transaction category
     *
     * @param menu The context menu to populate
     */
    void TransactionCategory::populateContextMenu(QMenu& menu)
    {
        using enum TransactionType;

        auto createName = [&](TransactionType type)
        { return "Create " + utils::toQString(type) + " Transaction"; };

        const auto depositName = createName(Deposit);
        _createDepositAction   = menu.addAction(depositName);
        _createDepositAction->setData(QVariant::fromValue(Deposit));

        const auto withdrawalName = createName(Withdrawal);
        _createWithdrawalAction   = menu.addAction(withdrawalName);
        _createWithdrawalAction->setData(QVariant::fromValue(Withdrawal));

        const auto stockTransactionName = createName(TransactionType::Stock);
        _createStockTransactionAction   = menu.addAction(stockTransactionName);
        _createStockTransactionAction->setData(
            QVariant::fromValue(TransactionType::Stock)
        );

        const auto optionTransactionName = createName(TransactionType::Option);
        _createOptionTransactionAction = menu.addAction(optionTransactionName);
        _createOptionTransactionAction->setData(
            QVariant::fromValue(TransactionType::Option)
        );
    }

}   // namespace ui