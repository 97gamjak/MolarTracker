#include "ui/securities/ticker_lookup.hpp"

#include <qlineedit.h>
#include <qpushbutton.h>

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "drafts/stock_draft.hpp"
#include "finance/ticker_info.hpp"
#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{

    /**
     * @brief Construct a new Ticker Lookup Widget:: Ticker Lookup Widget object
     *
     * @param parent The parent widget.
     */
    TickerLookupWidget::TickerLookupWidget(QWidget* parent)
        : QWidget(parent),
          _tickerInput(makeQChild<QLineEdit>(this)),
          _findButton(makeQChild<QPushButton>("Find", this)),
          _acceptButton(makeQChild<QPushButton>("Accept", this)),
          _symbolLabel(makeQChild<QLabel>(this)),
          _shortNameLabel(makeQChild<QLabel>(this)),
          _longNameLabel(makeQChild<QLabel>(this)),
          _currencyLabel(makeQChild<QLabel>(this)),
          _exchangeLabel(makeQChild<QLabel>(this)),
          _industryLabel(makeQChild<QLabel>(this)),
          _sectorLabel(makeQChild<QLabel>(this)),
          _assetClassLabel(makeQChild<QLabel>(this)),
          _statusLabel(makeQChild<QLabel>(this))
    {
        _tickerInput->setPlaceholderText("e.g. AAPL");
        constexpr auto maxLength = 12;
        _tickerInput->setMaxLength(maxLength);

        auto* searchRow = makeQChild<QHBoxLayout>();
        searchRow->addWidget(_tickerInput);
        searchRow->addWidget(_findButton);

        auto* resultForm = makeQChild<QFormLayout>();
        resultForm->addRow("Symbol:", _symbolLabel);
        resultForm->addRow("Short Name:", _shortNameLabel);
        resultForm->addRow("Long Name:", _longNameLabel);
        resultForm->addRow("Currency:", _currencyLabel);
        resultForm->addRow("Exchange:", _exchangeLabel);
        resultForm->addRow("Industry:", _industryLabel);
        resultForm->addRow("Sector:", _sectorLabel);
        resultForm->addRow("Asset Class:", _assetClassLabel);

        auto* statusRow = makeQChild<QHBoxLayout>();
        statusRow->addWidget(_statusLabel);

        auto* acceptRow = makeQChild<QHBoxLayout>();
        acceptRow->addWidget(_acceptButton);

        auto* root = makeQChild<QVBoxLayout>(this);
        root->addLayout(searchRow);
        root->addLayout(resultForm);
        root->addLayout(statusRow);
        root->addLayout(acceptRow);
        root->addStretch();

        clearResult();
    }

    /**
     * @brief Display the stock quote information.
     *
     * @param quote The stock quote information to display.
     */
    void TickerLookupWidget::displayQuote(const drafts::StockInfoDraft& quote)
    {
        _statusLabel->clear();

        _symbolLabel->setText(QString::fromStdString(quote.getTicker()));
        _shortNameLabel->setText(QString::fromStdString(quote.getShortName()));
        _longNameLabel->setText(QString::fromStdString(quote.getLongName()));
        _currencyLabel->setText(utils::toQString(quote.getCurrency()));
        _exchangeLabel->setText(QString::fromStdString(quote.getExchange()));
        _industryLabel->setText(QString::fromStdString(quote.getIndustry()));
        _sectorLabel->setText(QString::fromStdString(quote.getSector()));
        _assetClassLabel->setText(
            QString::fromStdString(finance::toString(quote.getAssetClass()))
        );

        _acceptButton->setEnabled(true);
    }

    /**
     * @brief Display an error message.
     *
     * @param message The error message to display.
     */
    void TickerLookupWidget::displayError(const std::string& message)
    {
        clearResult();
        _statusLabel->setText(QString::fromStdString(message));
        _acceptButton->setEnabled(false);
    }

    /**
     * @brief Clear the displayed stock quote information.
     *
     */
    void TickerLookupWidget::clearResult()
    {
        _symbolLabel->clear();
        _shortNameLabel->clear();
        _longNameLabel->clear();
        _currencyLabel->clear();
        _exchangeLabel->clear();
        _industryLabel->clear();
        _sectorLabel->clear();
        _assetClassLabel->clear();
        _statusLabel->clear();

        _acceptButton->setEnabled(false);
    }

    /**
     * @brief Get the ticker input from the user.
     *
     * @return std::string The ticker input.
     */
    std::string TickerLookupWidget::getTickerInput() const
    {
        return _tickerInput->text().toStdString();
    }

    /**
     * @brief Get the "Find" button.
     *
     * @return QPushButton* The "Find" button.
     */
    QPushButton* TickerLookupWidget::getFindButton() const
    {
        return _findButton;
    }

    /**
     * @brief Get the "Accept" button.
     *
     * @return QPushButton* The "Accept" button.
     */
    QPushButton* TickerLookupWidget::getAcceptButton() const
    {
        return _acceptButton;
    }

    void TickerLookupWidget::setTicker(const std::string& ticker)
    {
        clearResult();
        _tickerInput->setText(QString::fromStdString(ticker));
    }

}   // namespace ui