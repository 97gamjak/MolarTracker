// ticker_field.hpp
#ifndef __UI__INCLUDE__UI__TRANSACTION__TICKER_FIELD_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__TICKER_FIELD_HPP__

#include <qstring.h>
#include <qwidget.h>

#include <optional>
#include <vector>

class QLineEdit;     // Forward declaration
class QPushButton;   // Forward declaration
class QCompleter;    // Forward declaration

namespace ui
{

    /**
     * @brief Ticker search field with autocomplete and creation support
     *
     * Displays a line edit that filters available tickers as the user types,
     * using a QCompleter for the dropdown. A "+" button signals upward when
     * the user wants to create a new ticker, keeping the widget free of any
     * dialog or domain logic.
     */
    class TickerField : public QWidget
    {
        Q_OBJECT

       private:
        /// The list of available ticker symbols for autocomplete
        std::vector<QString> _tickers;

        /// The line edit for entering the ticker symbol
        QLineEdit* _lineEdit;

        /// The button for creating a new ticker
        QPushButton* _addButton;

        /// The completer for providing autocomplete suggestions based on the
        /// list of tickers
        QCompleter* _completer;

       public:
        explicit TickerField(
            std::vector<std::string> tickers,
            QWidget*                 parent = nullptr
        );

        void addTicker(QString ticker);

        [[nodiscard]] std::optional<QString> selected() const;

        void updateTickers(std::vector<QString> tickers);
        void selectTicker(const QString& ticker);

       signals:
        /**
         * @brief Emitted when the user selects a ticker from the completer
         *
         * @param ticker The selected ticker symbol
         */
        void tickerSelected(QString ticker);

        /**
         * @brief Emitted when the user clicks the "+" button to create a
         * new ticker
         */
        void createTickerRequested(std::string ticker);

       private:
        void _onTextEdited(const QString& text);
        void _onActivated(const QString& ticker);
        void _rebuildCompleter();
        void _onCreateTickerRequest();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__TICKER_FIELD_HPP__