#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__

#include <QObject>

#include "side_bar_category_controller.hpp"

namespace ui
{
    class StockOverviewWidget;   // Forward declaration
}   // namespace ui

namespace app
{
    class StockStore;   // Forward declaration
}   // namespace app

class QStackedWidget;   // Forward declaration

namespace controller
{

    /**
     * @brief Controller for the securities side bar.
     *
     * This class manages the interactions between the UI components and the
     * underlying data model for the securities side bar.
     */
    class SecuritiesSideBarController : public SideBarCategoryController
    {
        Q_OBJECT

       private:
        /// Widget for displaying stock overview infos
        ui::StockOverviewWidget* _stockOverviewWidget = nullptr;

        /// Reference to the stock store
        app::StockStore& _stockStore;

        /// Pointer to the stacked widget
        QStackedWidget* _stackedWidget;

       public:
        explicit SecuritiesSideBarController(
            QMainWindow*     mainWindow,
            app::StockStore& stockStore,
            QStackedWidget*  stackedWidget
        );

        void refresh() override;

        void onSecuritiesSelected();
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__
