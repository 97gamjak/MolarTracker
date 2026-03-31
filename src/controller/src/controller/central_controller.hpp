#ifndef __CONTROLLER__SRC__CONTROLLER__CENTRAL_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__CENTRAL_CONTROLLER_HPP__

#include <qstackedwidget.h>

#include <QObject>

#include "controller/account_controller.hpp"

namespace controller
{
    class CentralController : public QObject
    {
        Q_OBJECT

       private:
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
