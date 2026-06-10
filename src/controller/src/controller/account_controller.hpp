#ifndef __CONTROLLER__SRC__CONTROLLER__ACCOUNT_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__ACCOUNT_CONTROLLER_HPP__

#include <qtmetamacros.h>

#include <QObject>
#include <QPointer>
#include <memory>

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

namespace finance
{
    class PriceCache;   // Forward declaration
}   // namespace finance

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
        struct Details;
        /// Pointer to the details struct (PIMPL idiom)
        std::unique_ptr<Details> _details;

       public:
        AccountController(
            cmd::UndoStack&                                  undoStack,
            const std::shared_ptr<store::IAccountStore>&     accountStore,
            const std::shared_ptr<store::IPositionStore>&    positionStore,
            const std::shared_ptr<store::IStockStore>&       stockStore,
            const std::shared_ptr<store::ITransactionStore>& transactionStore,
            const std::shared_ptr<finance::PriceCache>&      priceCache,
            QStackedWidget*                                  stackedWidget
        );
        ~AccountController() override;

        void accountSelected(AccountId id);
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__ACCOUNT_CONTROLLER_HPP__