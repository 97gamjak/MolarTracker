// ticker_field.cpp
#include "ui/transaction/ticker_field.hpp"

#include <qboxlayout.h>
#include <qcompleter.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qstringlistmodel.h>

#include <algorithm>

#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{

    /**
     * @brief Construct a new Ticker Field:: Ticker Field object
     *
     * @param tickers A list of ticker symbols to initialize the field with
     * @param parent The parent widget for this field
     */
    TickerField::TickerField(std::vector<std::string> tickers, QWidget* parent)
        : QWidget(parent),
          _tickers(utils::toQStringVector(tickers)),
          _lineEdit(makeQChild<QLineEdit>(this)),
          _addButton(makeQChild<QPushButton>("+", this)),
          _completer(new QCompleter(this))
    {
        auto* layout = makeQChild<QHBoxLayout>();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(_lineEdit);
        layout->addWidget(_addButton);
        setLayout(layout);

        _lineEdit->setPlaceholderText("Search ticker...");

        constexpr int buttonSize = 28;
        _addButton->setFixedWidth(buttonSize);

        _completer->setCaseSensitivity(Qt::CaseInsensitive);
        _completer->setFilterMode(Qt::MatchContains);
        _completer->setCompletionMode(QCompleter::PopupCompletion);
        _lineEdit->setCompleter(_completer);

        _rebuildCompleter();

        connect(
            _lineEdit,
            &QLineEdit::textEdited,
            this,
            &TickerField::_onTextEdited
        );
        connect(
            _completer,
            QOverload<const QString&>::of(&QCompleter::activated),
            this,
            &TickerField::_onActivated
        );
        connect(
            _addButton,
            &QPushButton::clicked,
            this,
            &TickerField::_onCreateTickerRequest
        );
    }

    /**
     * @brief Get the currently selected ticker, this will return the ticker
     * symbol that is currently selected in the line edit, if it matches one of
     * the available tickers, allowing the owning dialog to determine which
     * ticker the user has selected for creating a transaction.
     *
     * @return std::optional<QString> The currently selected ticker symbol, or
     * std::nullopt if the text in the line edit does not match any available
     * tickers.
     */
    std::optional<QString> TickerField::selected() const
    {
        const auto text = _lineEdit->text().trimmed();

        const auto it = std::ranges::find(_tickers, text);

        return it != _tickers.end() ? std::optional{*it} : std::nullopt;
    }

    /**
     * @brief Add a new ticker to the field, this will add the given ticker
     * symbol to the list of available tickers and update the completer,
     * allowing the user to select this new ticker from the dropdown when they
     * type in the line edit.
     *
     * @param ticker The ticker symbol to add to the field, this should be a
     * valid ticker symbol that can be used for creating stock transactions.
     */
    void TickerField::addTicker(QString ticker)
    {
        _tickers.push_back(std::move(ticker));
        _rebuildCompleter();
    }

    /**
     * @brief Handle the text edited event for the line edit, this will check if
     * the current text matches a valid ticker and emit the tickerSelected
     * signal if it does, allowing the owning dialog to react to the user
     * selecting a ticker from the line edit.
     *
     * @param text The current text in the line edit after it has been edited,
     * this is used to determine if it matches one of the available tickers and
     * emit the appropriate signal if it does.
     */
    void TickerField::_onTextEdited(const QString& text)
    {
        if (text.trimmed().isEmpty())
            return;

        // clear selection if the user edited away from a valid ticker
        if (!selected().has_value())
            return;

        emit tickerSelected(*selected());
    }

    /**
     * @brief Handle the activated event from the completer, this will set the
     * text of the line edit to the selected ticker and emit the tickerSelected
     * signal, allowing the owning dialog to react to the user selecting a
     * ticker from the completer dropdown.
     *
     * @param ticker The ticker symbol that was selected from the completer,
     * this is used to update the line edit and emit the appropriate signal for
     * the owning dialog to react to.
     */
    void TickerField::_onActivated(const QString& ticker)
    {
        _lineEdit->setText(ticker);
        emit tickerSelected(ticker);
    }

    /**
     * @brief Rebuild the completer model based on the current list of tickers,
     * this will update the dropdown options in the completer to reflect any
     * changes to the list of available tickers, ensuring that the user always
     * sees an up-to-date list of tickers when they interact with the line edit.
     */
    void TickerField::_rebuildCompleter()
    {
        QStringList list;
        list.reserve(static_cast<qsizetype>(_tickers.size()));

        for (const auto& ticker : _tickers)
            list.append(ticker);

        _completer->setModel(
            utils::makeQChild<QStringListModel>(list, _completer)
        );
    }

    void TickerField::updateTickers(std::vector<QString> tickers)
    {
        _tickers = std::move(tickers);
        _rebuildCompleter();
    }

    void TickerField::_onCreateTickerRequest()
    {
        const auto text = _lineEdit->text().trimmed();
        emit       createTickerRequested(text.toStdString());
    }

    void TickerField::selectTicker(const QString& ticker)
    {
        _lineEdit->setText(ticker);
        _rebuildCompleter();
    }

}   // namespace ui