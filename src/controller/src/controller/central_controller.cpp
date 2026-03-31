#include "central_controller.hpp"

#include "controller/account_controller.hpp"
#include "utils/qt_helpers.hpp"

namespace controller
{
    /**
     * @brief Controller for managing the central part of the application
     *
     * @param undoStack A reference to the undo stack, this is used to push
     * commands that are created as a result of actions in the central part
     * of the application (e.g. creating a new account), this allows the user
     * to undo and redo actions related to the central part using the undo
     * stack.
     * @param appContext A reference to the application context, this is used to
     * access the stores and services needed to perform operations related to
     * the central part (e.g. creating a new account), this allows the
     * controller to interact with the underlying data and business logic for
     * the central part, and ensures that the controller can perform the
     * necessary operations to manage the central part effectively.
     * @param accountSideBarController A reference to the account side bar
     * controller, this is used to manage the account selection and display in
     * the side bar, and allows the controller to respond to account selection
     * events.
     * @param stackedWidget A pointer to the stacked widget, this is used to
     * manage the different views for the central part and other related
     * information, allowing the controller to switch between views as needed.
     */
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
