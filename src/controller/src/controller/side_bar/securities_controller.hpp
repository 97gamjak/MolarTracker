#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__

#include <QObject>
#include <memory>

#include "side_bar_category_controller.hpp"

namespace ui
{
    class StockOverviewWidget;   // Forward declaration
    class SecuritiesCategory;    // Forward declaration
    class TickerLookupWidget;    // Forward declaration
}   // namespace ui

namespace store
{
    class IStockStore;   // Forward declaration
}   // namespace store

namespace cache
{
    class StockCache;   // Forward declaration
}   // namespace cache

namespace finance
{
    class Stock;   // Forward declaration
}   // namespace finance

class QStackedWidget;   // Forward declaration
class QAction;          // Forward declaration
class Connections;      // Forward declaration

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
        struct UI;
        /// Pointer to the UI components of the controller
        std::unique_ptr<UI> _ui;

        /// Reference to the stock store
        std::shared_ptr<store::IStockStore> _stockStore;

        /// Reference to the stock cache
        std::shared_ptr<cache::StockCache> _stockCache;

        /// Connections for managing signal-slot connections
        std::unique_ptr<Connections> _connections;

       public:
        explicit SecuritiesSideBarController(
            QMainWindow*                               mainWindow,
            const std::shared_ptr<store::IStockStore>& stockStore,
            const std::shared_ptr<cache::StockCache>&  stockCache,
            QStackedWidget*                            stackedWidget
        );
        ~SecuritiesSideBarController() override;

        void onSecuritiesSelected();

        void handleContextMenuAction(
            const ui::SecuritiesCategory* category,
            QAction*                      action
        );

        void createStock(const std::string& ticker);

       signals:
        /**
         * @brief Emitted when a new stock is created
         *
         * @param stock The stock that was created
         */
        void stockCreated(const finance::Stock& stock);

       private:
        void _onFindTickerButtonClicked();
        void _onAcceptTickerButtonClicked();
        void _updateStockOverview();
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SECURITIES_CONTROLLER_HPP__
