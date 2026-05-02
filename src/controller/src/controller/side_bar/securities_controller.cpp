#include "securities_controller.hpp"

#include <qstackedwidget.h>

#include "app/store/stock_store.hpp"
#include "drafts/stock_mapper.hpp"
#include "ui/securities/stock_info_model.hpp"
#include "ui/securities/stock_overview.hpp"
#include "ui/side_bar/securities_category.hpp"

namespace controller
{
    /**
     * @brief Construct a new Securities Side Bar Controller:: Securities Side
     * Bar Controller object
     *
     * @param mainWindow
     * @param stockStore
     * @param stackedWidget
     */
    SecuritiesSideBarController::SecuritiesSideBarController(
        QMainWindow*     mainWindow,
        app::StockStore& stockStore,
        QStackedWidget*  stackedWidget
    )
        : SideBarCategoryController(new ui::SecuritiesCategory(), mainWindow),
          _stockOverviewWidget(new ui::StockOverviewWidget()),
          _stockStore(stockStore),
          _stackedWidget(stackedWidget)
    {
        _stackedWidget->addWidget(_stockOverviewWidget);
    }

    /**
     * @brief Refresh the securities overview.
     *
     */
    void SecuritiesSideBarController::refresh() {}

    /**
     * @brief Slot called when securities are selected.
     *
     */
    void SecuritiesSideBarController::onSecuritiesSelected()
    {
        const auto stocks =
            drafts::StockMapper::toStockInfoDrafts(_stockStore.getStocks());

        _stockOverviewWidget->getModel()->setRows(stocks);
        _stackedWidget->setCurrentWidget(_stockOverviewWidget);
    }

}   // namespace controller
