#ifndef __CONTROLLER__SRC__CONTROLLER__CENTRAL_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__CENTRAL_CONTROLLER_HPP__

#include <qstackedwidget.h>

#include <QObject>

#include "controller/account_controller.hpp"

namespace controller
{
    /**
     * @brief Controller for managing the central view of the application
     *
     */
    class CentralController : public QObject
    {
        Q_OBJECT

       private:
        /// Controller for managing account-related actions
        AccountController _accountController;

       public:
        CentralController(
            cmd::UndoStack&           undoStack,
            app::AppContext&          appContext,
            AccountSideBarController& accountSideBarController,
            QStackedWidget*           stackedWidget
        );
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__CENTRAL_CONTROLLER_HPP__
