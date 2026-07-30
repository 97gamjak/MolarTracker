#include "central_controller.hpp"

#include "common/qt_helpers.hpp"
#include "controller/account_controller.hpp"

namespace controller
{
    /**
     * @brief Controller for managing the central part of the application
     *
     * @param stackedWidget A pointer to the stacked widget, this is used to
     * manage the different views for the central part and other related
     * information, allowing the controller to switch between views as needed.
     */
    CentralController::CentralController(QStackedWidget* stackedWidget)
    {
        // TODO(97gamjak): make here at some point a nice overview
        auto* overView = common::makeQChild<QWidget>(stackedWidget);
        stackedWidget->addWidget(overView);
        stackedWidget->setCurrentWidget(overView);
    }
}   // namespace controller
