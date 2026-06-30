#include "securities_controller.hpp"

#include <qpushbutton.h>
#include <qstackedwidget.h>

#include "cache/stock_cache.hpp"
#include "controller/mapper/stock_mapper.hpp"
#include "finance/finance_error.hpp"
#include "finance/instrument/stock.hpp"
#include "store/i_stock_store.hpp"
#include "ui/securities/stock_info_model.hpp"
#include "ui/securities/stock_overview.hpp"
#include "ui/securities/ticker_lookup.hpp"
#include "ui/side_bar/securities_category.hpp"

namespace controller
{
    /**
     * @brief Struct representing the UI components of the
     * SecuritiesSideBarController.
     *
     */
    struct SecuritiesSideBarController::UI
    {
        /// Pointer to the stacked widget
        QStackedWidget* stackedWidget;
        /// Widget for displaying stock overview infos
        ui::StockOverviewWidget* stockOverviewWidget = nullptr;
        /// Widget for looking up stock tickers
        ui::TickerLookupWidget* tickerLookupWidget = nullptr;
        /// Optional accepted stock quote
        std::optional<finance::Stock> acceptedQuote = std::nullopt;

        explicit UI(QStackedWidget* stackedWidget);
    };

    /**
     * @brief Construct a new Securities Side Bar Controller:: UI object
     *
     * @param stackedWidget_ Pointer to the stacked widget
     */
    SecuritiesSideBarController::UI::UI(QStackedWidget* stackedWidget_)
        : stackedWidget(stackedWidget_),
          stockOverviewWidget(new ui::StockOverviewWidget()),
          tickerLookupWidget(new ui::TickerLookupWidget())
    {
        stackedWidget->addWidget(stockOverviewWidget);
    }

    /**
     * @brief Construct a new Securities Side Bar Controller:: Securities Side
     * Bar Controller object
     *
     * @param mainWindow
     * @param stockStore
     * @param stockCache
     * @param stackedWidget
     */
    SecuritiesSideBarController::SecuritiesSideBarController(
        QMainWindow*                               mainWindow,
        const std::shared_ptr<store::IStockStore>& stockStore,
        const std::shared_ptr<cache::StockCache>&  stockCache,
        QStackedWidget*                            stackedWidget
    )
        : SideBarCategoryController(new ui::SecuritiesCategory(), mainWindow),
          _ui(std::make_unique<UI>(stackedWidget)),
          _stockStore(stockStore),
          _stockCache(stockCache),
          _connections(std::make_unique<Connections>())
    {
        _ui->stackedWidget->addWidget(_ui->stockOverviewWidget);

        connect(
            _ui->tickerLookupWidget->getFindButton(),
            &QPushButton::clicked,
            this,
            &SecuritiesSideBarController::_onFindTickerButtonClicked
        );

        connect(
            _ui->tickerLookupWidget->getAcceptButton(),
            &QPushButton::clicked,
            this,
            &SecuritiesSideBarController::_onAcceptTickerButtonClicked
        );

        _connections->add(_stockCache->subscribeToChanged(
            [this]() { _updateStockOverview(); },
            this
        ));
    }

    SecuritiesSideBarController::~SecuritiesSideBarController() = default;

    /**
     * @brief Slot called when securities are selected.
     *
     */
    void SecuritiesSideBarController::onSecuritiesSelected()
    {
        _updateStockOverview();
        _ui->stackedWidget->setCurrentWidget(_ui->stockOverviewWidget);
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
            createStock("");
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
        const auto ticker = _ui->tickerLookupWidget->getTickerInput();

        const auto result = finance::Stock::retrieveTickerInfo(ticker);

        if (!result)
        {
            _ui->tickerLookupWidget->displayError(result.error().toString());
            _ui->acceptedQuote = std::nullopt;
            return;
        }

        _ui->acceptedQuote = result.value();
        _ui->tickerLookupWidget->displayQuote(
            StockMapper::toStockInfoDraft(result.value())
        );
    }

    /**
     * @brief Slot called when the accept ticker button is clicked.
     *
     */
    void SecuritiesSideBarController::_onAcceptTickerButtonClicked()
    {
        if (_ui->acceptedQuote)
        {
            const auto result =
                _stockStore->addStock(_ui->acceptedQuote.value());

            if (result != store::StockStoreResult::Ok)
                _ui->tickerLookupWidget->displayError("Failed to add stock");
        }

        _ui->tickerLookupWidget->clearResult();
        _ui->tickerLookupWidget->hide();

        if (_ui->acceptedQuote)
            emit stockCreated(_ui->acceptedQuote.value());
    }

    /**
     * @brief Create a new stock with the given ticker symbol.
     *
     * @param ticker The ticker symbol of the stock to create
     */
    void SecuritiesSideBarController::createStock(const std::string& ticker)
    {
        _ui->tickerLookupWidget->setTicker(ticker);
        _ui->tickerLookupWidget->show();
    }

    /**
     * @brief Update the stock overview widget with the latest stock information
     * from the cache.
     *
     */
    void SecuritiesSideBarController::_updateStockOverview()
    {
        const auto stocks =
            StockMapper::toStockInfoDrafts(_stockCache->getAllStocks());

        _ui->stockOverviewWidget->getModel()->setRows(stocks);
    }

}   // namespace controller
