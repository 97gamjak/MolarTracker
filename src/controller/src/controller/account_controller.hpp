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
    class AccountDraft;   // Forward declaration
}   // namespace drafts

namespace store
{
    class IAccountStore;       // Forward declaration
    class IPositionStore;      // Forward declaration
    class IStockStore;         // Forward declaration
    class ITransactionStore;   // Forward declaration
}   // namespace store

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
            cmd::UndoStack&                                  undoStack,
            const std::shared_ptr<store::IAccountStore>&     accountStore,
            const std::shared_ptr<store::IPositionStore>&    positionStore,
            const std::shared_ptr<store::IStockStore>&       stockStore,
            const std::shared_ptr<store::ITransactionStore>& transactionStore,
            QStackedWidget*                                  stackedWidget
        );
        ~AccountController() override;

        void accountSelected(AccountId id);
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__ACCOUNT_CONTROLLER_HPP__