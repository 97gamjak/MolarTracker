#ifndef __UI__INCLUDE__UI__SECURITIES__TICKER_LOOKUP_HPP__
#define __UI__INCLUDE__UI__SECURITIES__TICKER_LOOKUP_HPP__

#include <QWidget>

#include "drafts/stock_draft.hpp"

class QLineEdit;
class QLabel;
class QPushButton;

namespace draft
{
    class StockInfoDraft;   // Forward declaration
}   // namespace draft

namespace ui
{

    /**
     * @brief Widget for looking up stock tickers
     *
     */
    class TickerLookupWidget : public QWidget
    {
        Q_OBJECT
       private:
        /// Input field for the stock ticker symbol
        QLineEdit* _tickerInput;
        /// Button to find the stock information
        QPushButton* _findButton;
        /// Button to accept the stock information
        QPushButton* _acceptButton;

        // Result area
        /// Label for the stock ticker symbol
        QLabel* _symbolLabel;
        /// Label for the stock short name
        QLabel* _shortNameLabel;
        /// Label for the stock long name
        QLabel* _longNameLabel;
        /// Label for the stock currency
        QLabel* _currencyLabel;
        /// Label for the stock exchange
        QLabel* _exchangeLabel;
        /// Label for the stock industry
        QLabel* _industryLabel;
        /// Label for the stock sector
        QLabel* _sectorLabel;
        /// Label for the stock asset class
        QLabel* _assetClassLabel;

        /// Label for the stock status
        QLabel* _statusLabel;

       public:
        explicit TickerLookupWidget(QWidget* parent = nullptr);

        [[nodiscard]] std::string  getTickerInput() const;
        [[nodiscard]] QPushButton* getFindButton() const;
        [[nodiscard]] QPushButton* getAcceptButton() const;

        void displayQuote(const drafts::StockInfoDraft& quote);
        void displayError(const std::string& message);
        void clearResult();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SECURITIES__TICKER_LOOKUP_HPP__
