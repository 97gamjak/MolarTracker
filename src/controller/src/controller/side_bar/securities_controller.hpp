#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__

#include <QObject>

#include "finance/stock.hpp"
#include "side_bar_category_controller.hpp"

namespace ui
{
    class StockOverviewWidget;   // Forward declaration
    class SecuritiesCategory;    // Forward declaration
    class TickerLookupWidget;    // Forward declaration
}   // namespace ui

namespace app
{
    class StockStore;   // Forward declaration
}   // namespace app

class QStackedWidget;   // Forward declaration
class QAction;          // Forward declaration

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
        /// Widget for looking up stock tickers
        ui::TickerLookupWidget* _tickerLookupWidget = nullptr;

        /// Optional accepted stock quote
        std::optional<finance::Stock> _acceptedQuote = std::nullopt;

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

        void handleContextMenuAction(
            const ui::SecuritiesCategory* category,
            QAction*                      action
        );

       private:
        void _onFindTickerButtonClicked();
        void _onAcceptTickerButtonClicked();
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__
