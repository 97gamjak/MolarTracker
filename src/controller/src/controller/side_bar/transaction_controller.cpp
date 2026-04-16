#include "transaction_controller.hpp"

#include "config/finance.hpp"
#include "logging/log_macros.hpp"
#include "ui/side_bar/transaction_category.hpp"
#include "utils/qt_helpers.hpp"

REGISTER_LOG_CATEGORY("Controller.SideBar.TransactionSideBarController");

namespace controller
{
    /**
     * @brief Construct a new Transaction Side Bar Controller:: Transaction Side
     * Bar Controller object
     *
     * @param undoStack The undo stack for the application
     * @param appContext The application context
     * @param mainWindow The main window of the application
     */
    TransactionSideBarController::TransactionSideBarController(
        cmd::UndoStack&  undoStack,
        app::AppContext& appContext,
        QMainWindow*     mainWindow
    )
        : SideBarCategoryController(new ui::TransactionCategory(), mainWindow),
          _undoStack(undoStack),
          _appContext(appContext)
    {
    }

    /**
     * @brief Refresh the transaction category
     *
     */
    void TransactionSideBarController::refresh() {}

    /**
     * @brief Handle a context menu action for the transaction category
     *
     * @param item The transaction category item
     * @param action The action that was triggered
     */
    void TransactionSideBarController::handleContextMenuAction(
        const ui::TransactionCategory* item,
        const QAction*                 action
    )
    {
        if (item == nullptr || action == nullptr)
        {
            LOG_WARNING(
                "Context menu requested with null item or action, ignoring"
            );
            return;
        }

        if (action == item->getCreateAction() ||
            action == item->getCreateDepositAction() ||
            action == item->getCreateWithdrawalAction())
        {
            LOG_DEBUG("Create action triggered for transaction category");

            if (action == item->getCreateAction())
            {
                _createTransactionDialog =
                    utils::makeQChild<ui::CreateTransactionDialog>(
                        getMainWindow()
                    );
            }
            else
            {
                _createTransactionDialog =
                    utils::makeQChild<ui::CreateTransactionDialog>(
                        getMainWindow(),
                        action->data().value<TransactionType>()
                    );
            }

            if (auto* dialog = _createTransactionDialog.data())
                dialog->exec();
        }
    }
}   // namespace controller
