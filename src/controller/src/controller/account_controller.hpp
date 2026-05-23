#ifndef __CONTROLLER__SRC__CONTROLLER__ACCOUNT_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__ACCOUNT_CONTROLLER_HPP__

#include <qtmetamacros.h>

#include <QObject>
#include <QPointer>

#include "config/id_types.hpp"

class QAction;          // Forward declaration
class QMainWindow;      // Forward declaration
class QStackedWidget;   // Forward declaration

namespace drafts
{
    struct AccountDraft;   // Forward declaration
}   // namespace drafts

namespace app
{
    class AccountStore;       // Forward declaration
    class PositionStore;      // Forward declaration
    class StockStore;         // Forward declaration
    class TransactionStore;   // Forward declaration
}   // namespace app

namespace ui
{
    class AccountCategory;       // Forward declaration
    class CreateAccountDialog;   // Forward declaration
    class AccountDetailView;     // Forward declaration
}   // namespace ui

namespace cmd
{
    class UndoStack;   // Forward declaration
}   // namespace cmd

namespace controller
{
    /**
     * @brief Controller for managing account details
     *
     */
    class AccountController : public QObject
    {
        Q_OBJECT

       private:
        /// Reference to the undo stack
        cmd::UndoStack& _undoStack;

        struct Stores;
        /// Pointer to the stores
        std::unique_ptr<Stores> _stores;

        /// Pointer to the stacked widget
        QStackedWidget* _stackedWidget;
        /// Pointer to the account detail view
        QPointer<ui::AccountDetailView> _accountDetailView;

       public:
        AccountController(
            cmd::UndoStack&        undoStack,
            app::AccountStore&     accountStore,
            app::PositionStore&    positionStore,
            app::StockStore&       stockStore,
            app::TransactionStore& transactionStore,
            QStackedWidget*        stackedWidget
        );
        ~AccountController() override;

        void accountSelected(AccountId id);
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__ACCOUNT_CONTROLLER_HPP__