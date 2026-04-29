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

    TickerField::TickerField(std::vector<QString> tickers, QWidget* parent)
        : QWidget(parent),
          _tickers(std::move(tickers)),
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

        _addButton->setFixedWidth(28);

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
            &TickerField::createTickerRequested
        );
    }

    std::optional<QString> TickerField::selected() const
    {
        const auto text = _lineEdit->text().trimmed();

        const auto it = std::ranges::find(_tickers, text);

        return it != _tickers.end() ? std::optional{*it} : std::nullopt;
    }

    void TickerField::addTicker(QString ticker)
    {
        _tickers.push_back(std::move(ticker));
        _rebuildCompleter();
    }

    void TickerField::_onTextEdited(const QString& text)
    {
        if (text.trimmed().isEmpty())
            return;

        // clear selection if the user edited away from a valid ticker
        if (!selected().has_value())
            return;

        emit tickerSelected(*selected());
    }

    void TickerField::_onActivated(const QString& ticker)
    {
        _lineEdit->setText(ticker);
        emit tickerSelected(ticker);
    }

    void TickerField::_rebuildCompleter()
    {
        QStringList list;
        list.reserve(static_cast<qsizetype>(_tickers.size()));

        for (const auto& ticker : _tickers)
            list.append(ticker);

        _completer->setModel(new QStringListModel(list, _completer));
    }

}   // namespace ui