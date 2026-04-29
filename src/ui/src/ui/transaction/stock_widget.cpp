// stock_widget.cpp
#include "ui/transaction/stock_widget.hpp"

#include <qboxlayout.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include "finance/currency.hpp"
#include "ui/transaction/account_combo.hpp"
#include "ui/transaction/amount_row.hpp"
#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{

    StockWidget::StockWidget(
        TransactionType                          type,
        std::vector<drafts::AccountDraft>        accounts,
        const std::vector<drafts::AccountDraft>& referenceAccounts,
        QWidget*                                 parent
    )
        : ICreateTransactionWidget(parent),
          _type(type),
          _layout(new QFormLayout(this)),
          _accountCombo(makeQChild<AccountCombo>(std::move(accounts), this)),
          _referenceAccountCombo(
              makeQChild<AccountCombo>(referenceAccounts, this)
          ),
          _quantityRow(makeQChild<AmountRow>(this)),
          _priceRow(makeQChild<AmountRow>(this)),
          _currencyLabel(makeQChild<QLabel>(this)),
          _addButton(makeQChild<QPushButton>("Add Transaction", this)),
          _tickerField(
              makeQChild<TickerField>(std::vector<QString>{"GOOG"}, this)
          ),
          _referenceAccounts(referenceAccounts)
    {
        setLayout(_layout);

        _layout->addRow("Account:", _accountCombo);
        _layout->addRow("Reference Account:", _referenceAccountCombo);
        _layout->addRow("Ticker:", _tickerField);

        auto* quantityRowLayout = makeQChild<QHBoxLayout>();
        quantityRowLayout->addWidget(_quantityRow);
        _layout->addRow("Quantity:", quantityRowLayout);
        _quantityRow->setNDecimalPlaces(6);

        auto* amountRowLayout = makeQChild<QHBoxLayout>();
        amountRowLayout->addWidget(_priceRow);
        amountRowLayout->addWidget(_currencyLabel);
        _layout->addRow("Stock Price:", amountRowLayout);

        _referenceAccountCombo->setEnabled(false);

        _addButton->setEnabled(false);
        auto* buttonLayout = makeQChild<QHBoxLayout>();
        buttonLayout->addWidget(_addButton);
        _layout->addRow(buttonLayout);

        connect(
            _accountCombo,
            &AccountCombo::accountSelected,
            this,
            &StockWidget::_onAccountSelected
        );
        connect(
            _referenceAccountCombo,
            &AccountCombo::accountSelected,
            this,
            &StockWidget::_onReferenceAccountSelected
        );
        connect(
            _priceRow,
            &AmountRow::validityChanged,
            this,
            &StockWidget::_updateAddButton
        );
        connect(
            _priceRow,
            &AmountRow::valueChanged,
            this,
            &StockWidget::_updateAddButton
        );
        connect(_addButton, &QPushButton::clicked, this, &StockWidget::_emitOk);
    }

    TransactionType StockWidget::getTransactionType() const { return _type; }

    void StockWidget::_onAccountSelected(const drafts::AccountDraft& account)
    {
        using finance::getMicroUnit;
        using finance::getSymbol;

        _priceRow->setNDecimalPlaces(getMicroUnit(account.currency));
        _currencyLabel->setText(getSymbol(account.currency).c_str());

        std::vector<drafts::AccountDraft> referenceAccounts;

        for (const auto& referenceAccount : _referenceAccounts)
        {
            if (referenceAccount.currency == account.currency)
                referenceAccounts.push_back(referenceAccount);
        }

        _referenceAccountCombo->resetSelection();
        _referenceAccountCombo->updateAccounts(std::move(referenceAccounts));
        _referenceAccountCombo->setEnabled(true);
        _updateAddButton();
    }

    void StockWidget::_onReferenceAccountSelected(
        const drafts::AccountDraft& /*account*/
    )
    {
        _updateAddButton();
    }

    void StockWidget::_updateAddButton()
    {
        const auto isValid =
            _accountCombo->selected().has_value() &&
            _referenceAccountCombo->selected().has_value() &&
            _priceRow->isValid() && _quantityRow->getAmount() != 0 &&
            _priceRow->getAmount() != 0 && _quantityRow->isValid();

        _addButton->setEnabled(isValid);
    }

    void StockWidget::_emitOk() { emit createTransactionRequested(_type); }

}   // namespace ui