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

       public:
        explicit CentralController(QStackedWidget* stackedWidget);
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__CENTRAL_CONTROLLER_HPP__
