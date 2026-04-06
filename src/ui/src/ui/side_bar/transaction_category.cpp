#include "ui/side_bar/transaction_category.hpp"

#include <QMenu>

#include "ui/side_bar/category.hpp"

namespace ui
{
    /**
     * @brief Construct a new Transaction Category:: Transaction Category object
     *
     */
    TransactionCategory::TransactionCategory()
        : Category("Transactions", SideBarItemType::TransactionCategory),
          _createAction(nullptr)
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

    /**
     * @brief Populate the context menu for the transaction category
     *
     * @param menu The context menu to populate
     */
    void TransactionCategory::populateContextMenu(QMenu& menu)
    {
        _createAction = menu.addAction("Create Transaction");
    }

}   // namespace ui