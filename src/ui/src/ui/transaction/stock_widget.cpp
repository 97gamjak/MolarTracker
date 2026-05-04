// stock_widget.cpp
#include "ui/transaction/stock_widget.hpp"

#include <qboxlayout.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include "drafts/transaction_draft.hpp"
#include "finance/cash.hpp"
#include "finance/currency.hpp"
#include "ui/transaction/account_combo.hpp"
#include "ui/transaction/amount_row.hpp"
#include "ui/transaction/comment_field.hpp"
#include "ui/transaction/ticker_field.hpp"
#include "ui/transaction/timestamp_field.hpp"
#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{

    /**
     * @brief Construct a new Stock Widget:: Stock Widget object
     *
     * @param accounts A list of account drafts to populate the account combo
     * box
     * @param referenceAccounts A list of account drafts to populate the
     * reference account combo box, this will be filtered based on the selected
     * primary account to only include accounts with the same currency
     * @param tickers A list of ticker symbols to populate the ticker field
     * @param parent The parent widget for this widget
     */
    StockWidget::StockWidget(
        std::vector<drafts::AccountDraft>        accounts,
        const std::vector<drafts::AccountDraft>& referenceAccounts,
        std::vector<std::string>                 tickers,
        QWidget*                                 parent
    )
        : Dialog(parent),
          _layout(new QFormLayout(this)),
          _accountCombo(makeQChild<AccountCombo>(std::move(accounts), this)),
          _referenceAccountCombo(
              makeQChild<AccountCombo>(referenceAccounts, this)
          ),
          _quantityRow(makeQChild<AmountRow>(this)),
          _priceRow(makeQChild<AmountRow>(this)),
          _currencyLabel(makeQChild<QLabel>(this)),
          _addButton(makeQChild<QPushButton>("Add Transaction", this)),
          _tickerField(makeQChild<TickerField>(std::move(tickers), this)),
          _timestampField(makeQChild<TimestampField>(this)),
          _commentField(makeQChild<CommentField>(this)),
          _referenceAccounts(referenceAccounts)
    {
        setLayout(_layout);

        _layout->addRow("Account:", _accountCombo);
        _layout->addRow("Reference Account:", _referenceAccountCombo);
        _layout->addRow("Ticker:", _tickerField);
        _layout->addRow("Timestamp:", _timestampField);

        auto* quantityRowLayout = makeQChild<QHBoxLayout>();
        quantityRowLayout->addWidget(_quantityRow);
        _layout->addRow("Quantity:", quantityRowLayout);
        _quantityRow->setNDecimalPlaces(Quantity::precision);

        auto* amountRowLayout = makeQChild<QHBoxLayout>();
        amountRowLayout->addWidget(_priceRow);
        amountRowLayout->addWidget(_currencyLabel);
        _layout->addRow("Stock Price:", amountRowLayout);
        _layout->addRow("Comment:", _commentField);

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
        _connectAddButton();
        connect(
            _tickerField,
            &TickerField::createTickerRequested,
            this,
            &StockWidget::createTickerRequested
        );

        connect(_addButton, &QPushButton::clicked, this, &StockWidget::_emitOk);
    }

    /**
     * @brief Handle the selection of a primary account, this will be called
     * when the user selects a primary account from the account combo box, and
     * should handle updating the UI to reflect the selected account (e.g.
     * updating the currency label to show the currency of the selected
     * account), as well as filtering the reference accounts to only include
     * accounts with the same currency as the selected primary account, ensuring
     * that the user can only select valid reference accounts for the stock
     * transaction.
     *
     * @param account The account draft of the selected primary account, this is
     * used to determine the currency of the selected account for updating the
     * UI and filtering the reference accounts.
     */
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

        _referenceAccountCombo->updateAccounts(std::move(referenceAccounts));
        _referenceAccountCombo->setEnabled(true);
        _updateAddButton();
    }

    /**
     * @brief Handle the selection of a reference account, this will be called
     * when the user selects a reference account from the reference account
     * combo box, and should handle updating the UI to reflect the selected
     * reference account (if necessary), as well as enabling the add button if
     * all required fields are valid and filled out, allowing the user to
     * proceed with creating the stock transaction once they have selected a
     * valid reference account.
     *
     * @param account The account draft of the selected reference account, this
     * is used to determine if a valid reference account has been selected for
     * enabling the add button.
     */
    void StockWidget::_onReferenceAccountSelected(
        const drafts::AccountDraft& /*account*/
    )
    {
        _updateAddButton();
    }

    /**
     * @brief Update the enabled state of the add button, this should check if
     * all required fields for creating a stock transaction are valid and filled
     * out (e.g. a primary account is selected, a reference account is selected,
     * the quantity and price are valid and non-zero), and then enable or
     * disable the add button accordingly, ensuring that the user can only
     * proceed with creating the stock transaction once all necessary
     * information has been provided and is valid.
     */
    void StockWidget::_updateAddButton()
    {
        const auto isValid = _accountCombo->selected().has_value() &&
                             _referenceAccountCombo->selected().has_value() &&
                             _priceRow->isValid() &&
                             _quantityRow->getAmount() != 0 &&
                             _priceRow->getAmount() != 0 &&
                             _quantityRow->isValid() && _tickerField->isValid();

        _addButton->setEnabled(isValid);
    }

    /**
     * @brief Emit the signal to create the transaction, this will be called
     * when the user clicks the add button, and should emit a signal with the
     * transaction draft generated from the information entered in this widget,
     * allowing the owning dialog to handle creating the transaction in the
     * store based on the provided draft. This allows the widget to communicate
     * with the rest of the UI and trigger the creation of a new stock
     * transaction once the user has entered all necessary information and is
     * ready to proceed.
     */
    void StockWidget::_emitOk()
    {
        emit createStockTransactionRequested(_getDraft());
    }

    /**
     * @brief Update the list of accounts in the account combo box
     *
     * @param accounts The new list of account drafts to populate the combo box
     */
    void StockWidget::updateAccounts(std::vector<drafts::AccountDraft> accounts)
    {
        _accountCombo->updateAccounts(std::move(accounts));
    }

    /**
     * @brief Update the list of reference accounts in the reference account
     * combo box
     *
     * @param referenceAccounts The new list of reference account drafts to
     * populate the combo box
     */
    void StockWidget::updateReferenceAccounts(
        std::vector<drafts::AccountDraft> referenceAccounts
    )
    {
        _referenceAccounts = std::move(referenceAccounts);

        const auto account = _accountCombo->selected();
        if (account.has_value())
            _onAccountSelected(account.value());
    }

    /**
     * @brief Update the list of tickers in the ticker field
     *
     * @param tickers The new list of ticker symbols to populate the ticker
     * field
     * @param tickerToSelect The ticker symbol to select in the ticker field
     */
    void StockWidget::updateTickers(const std::vector<std::string>& tickers)
    {
        std::vector<QString> qTickers;
        qTickers.reserve(tickers.size());
        for (const auto& ticker : tickers)
            qTickers.emplace_back(QString::fromStdString(ticker));

        _tickerField->updateTickers(std::move(qTickers));
    }

    /**
     * @brief Refresh the widget to reflect the current state
     *
     */
    void StockWidget::refresh()
    {
        _accountCombo->update();
        _referenceAccountCombo->update();
        _quantityRow->update();
        _priceRow->update();
        _currencyLabel->update();
        _addButton->update();
        _tickerField->update();
    }

    drafts::CreateStockTransactionDraft StockWidget::_getDraft() const
    {
        const auto account = _accountCombo->selected();

        if (!account.has_value() || account.value().id == AccountId::invalid())
            throw std::runtime_error("No account selected");

        const auto referenceAccount = _referenceAccountCombo->selected();

        if (!referenceAccount.has_value() ||
            referenceAccount.value().id == AccountId::invalid())
            throw std::runtime_error("No reference account selected");

        const auto unitPrice =
            finance::Cash(referenceAccount->currency, _priceRow->getAmount());

        const auto quantity = Quantity{_quantityRow->getAmount()};

        const auto cash = -quantity * unitPrice;

        auto entry = drafts::TransactionEntryDraft{referenceAccount->id, cash};

        const auto ticker = _tickerField->getTicker();
        if (!ticker.has_value())
            throw std::runtime_error("No ticker selected");

        auto tradeLeg = drafts::TradeLegDraft{
            referenceAccount->id,
            unitPrice,
            quantity,
            ticker.value()
        };

        return {
            _timestampField->getTimestamp(),
            {entry},
            {tradeLeg},
            _commentField->getComment()
        };
    }

    void StockWidget::_connectAddButton()
    {
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
        connect(
            _tickerField,
            &TickerField::tickerSelected,
            this,
            &StockWidget::_updateAddButton
        );
    }

}   // namespace ui