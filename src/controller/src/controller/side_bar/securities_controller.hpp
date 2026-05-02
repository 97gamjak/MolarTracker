#ifndef __CONTROLLER__INCLUDE__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__
#define __CONTROLLER__INCLUDE__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__

#include <QObject>

#include "side_bar_category_controller.hpp"

namespace controller
{

    class SecuritiesSideBarController : public SideBarCategoryController
    {
        Q_OBJECT
       public:
        explicit SecuritiesSideBarController(QMainWindow* mainWindow);

        void refresh() override;

        void onSecuritiesSelected();
    };

}   // namespace controller

#endif   // __CONTROLLER__INCLUDE__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__
