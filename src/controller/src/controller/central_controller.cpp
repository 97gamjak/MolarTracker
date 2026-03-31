#include "central_controller.hpp"

#include "controller/account_controller.hpp"
#include "utils/qt_helpers.hpp"

namespace controller
{
    CentralController::CentralController(
        cmd::UndoStack&           undoStack,
        app::AppContext&          appContext,
        AccountSideBarController& accountSideBarController,
        QStackedWidget*           stackedWidget
    )
        : _accountController(
              undoStack,
              appContext,
              accountSideBarController,
              stackedWidget
          )
    {
        // TODO(97gamjak): make here at some point a nice overview
        auto* overView = utils::makeQChild<QWidget>(stackedWidget);
        stackedWidget->addWidget(overView);
        stackedWidget->setCurrentWidget(overView);
    }
}   // namespace controller
