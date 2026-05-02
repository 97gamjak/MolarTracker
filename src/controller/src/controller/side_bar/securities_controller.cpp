#include "securities_controller.hpp"

#include <qpushbutton.h>
#include <qstackedwidget.h>

#include "app/store/stock_store.hpp"
#include "drafts/stock_mapper.hpp"
#include "finance/stock.hpp"
#include "http/http_error.hpp"
#include "ui/securities/stock_info_model.hpp"
#include "ui/securities/stock_overview.hpp"
#include "ui/securities/ticker_lookup.hpp"
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
          _tickerLookupWidget(new ui::TickerLookupWidget()),
          _stockStore(stockStore),
          _stackedWidget(stackedWidget)
    {
        _stackedWidget->addWidget(_stockOverviewWidget);

        connect(
            _tickerLookupWidget->getFindButton(),
            &QPushButton::clicked,
            this,
            &SecuritiesSideBarController::_onFindTickerButtonClicked
        );

        connect(
            _tickerLookupWidget->getAcceptButton(),
            &QPushButton::clicked,
            this,
            &SecuritiesSideBarController::_onAcceptTickerButtonClicked
        );
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

    /**
     * @brief Handle context menu actions for the securities category.
     *
     * @param category The securities category
     * @param action The action triggered
     */
    void SecuritiesSideBarController::handleContextMenuAction(
        const ui::SecuritiesCategory* category,
        QAction*                      action
    )
    {
        if (action == category->getCreateAction())
        {
            _tickerLookupWidget->show();
        }
        else
        {
            throw std::logic_error("Context menu action not handled");
        }
    }

    /**
     * @brief Slot called when the find ticker button is clicked.
     *
     */
    void SecuritiesSideBarController::_onFindTickerButtonClicked()
    {
        const auto ticker = _tickerLookupWidget->getTickerInput();

        const auto result = finance::Stock::retrieveTickerInfo(ticker);

        if (!result)
        {
            _tickerLookupWidget->displayError(result.error().message);
            _acceptedQuote = std::nullopt;
            return;
        }

        _acceptedQuote = result.value();
        _tickerLookupWidget->displayQuote(
            drafts::StockMapper::toStockInfoDraft(result.value())
        );
    }

    /**
     * @brief Slot called when the accept ticker button is clicked.
     *
     */
    void SecuritiesSideBarController::_onAcceptTickerButtonClicked()
    {
        if (_acceptedQuote)
            _stockStore.addStock(_acceptedQuote.value());

        _tickerLookupWidget->clearResult();
        _tickerLookupWidget->hide();
    }

}   // namespace controller
