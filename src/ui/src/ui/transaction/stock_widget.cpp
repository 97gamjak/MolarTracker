// stock_widget.cpp
#include "ui/transaction/stock_widget.hpp"

#include <qboxlayout.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qwidget.h>

#include "config/finance.hpp"
#include "drafts/account_draft.hpp"
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

    struct StockWidget::Fields
    {
        /// The combo box for selecting the primary account
        AccountCombo* accountCombo;

        /// The combo box for selecting the reference account
        AccountCombo* referenceAccountCombo;

        /// The row for entering the quantity of the stock
        AmountRow* quantityRow;

        /// The row for entering the price of the stock
        AmountRow* priceRow;

        /// The row for entering the fees of the stock
        AmountRow* feesRow;

        /// The label for displaying the currency of the selected account
        QLabel* currencyLabel;

        /// The field for entering the stock ticker
        TickerField* tickerField;

        /// The field for entering the timestamp of the transaction
        TimestampField* timestampField;

        /// The field for entering a comment about the transaction
        CommentField* commentField;

        Fields(
            const std::vector<drafts::AccountDraft>& accounts,
            const std::vector<drafts::AccountDraft>& referenceAccounts,
            const std::vector<std::string>&          tickers,
            QWidget*                                 parent
        );

        void addFieldsToLayout(QFormLayout* layout) const;

        [[nodiscard]]
        bool isValid() const;

        void update() const;

        [[nodiscard]]
        drafts::CreateStockTransactionDraft getDraft() const;
    };

    StockWidget::Fields::Fields(
        const std::vector<drafts::AccountDraft>& accounts,
        const std::vector<drafts::AccountDraft>& referenceAccounts,
        const std::vector<std::string>&          tickers,
        QWidget*                                 parent
    )
        : accountCombo(makeQChild<AccountCombo>(accounts, parent)),
          referenceAccountCombo(
              makeQChild<AccountCombo>(referenceAccounts, parent)
          ),
          quantityRow(makeQChild<AmountRow>(parent)),
          priceRow(makeQChild<AmountRow>(parent)),
          feesRow(makeQChild<AmountRow>(parent)),
          currencyLabel(makeQChild<QLabel>(parent)),
          tickerField(makeQChild<TickerField>(tickers, parent)),
          timestampField(makeQChild<TimestampField>(parent)),
          commentField(makeQChild<CommentField>(parent))
    {
        feesRow->setDefaultValue(0);
    }

    void StockWidget::Fields::addFieldsToLayout(QFormLayout* layout) const
    {
        layout->addRow("Account:", accountCombo);
        layout->addRow("Reference Account:", referenceAccountCombo);
        layout->addRow("Ticker:", tickerField);
        layout->addRow("Timestamp:", timestampField);

        auto* quantityRowLayout = makeQChild<QHBoxLayout>();
        quantityRowLayout->addWidget(quantityRow);
        layout->addRow("Quantity:", quantityRowLayout);
        quantityRow->setNDecimalPlaces(Quantity::precision);

        auto* amountRowLayout = makeQChild<QHBoxLayout>();
        amountRowLayout->addWidget(priceRow);
        amountRowLayout->addWidget(currencyLabel);
        layout->addRow("Stock Price:", amountRowLayout);
        auto* feesRowLayout = makeQChild<QHBoxLayout>();
        feesRowLayout->addWidget(feesRow);
        layout->addRow("Fees:", feesRowLayout);
        layout->addRow("Comment:", commentField);

        referenceAccountCombo->setEnabled(false);
    }

    bool StockWidget::Fields::isValid() const
    {
        if (!accountCombo->selected().has_value())
            return false;
        if (!referenceAccountCombo->selected().has_value())
            return false;

        if (!tickerField->isValid())
            return false;
        if (!quantityRow->isValid())
            return false;
        if (!priceRow->isValid())
            return false;

        if (quantityRow->getAmount() == 0)
            return false;

        if (priceRow->getAmount() == 0)
            return false;

        return true;
    }

    void StockWidget::Fields::update() const
    {
        accountCombo->update();
        referenceAccountCombo->update();
        quantityRow->update();
        priceRow->update();
        feesRow->update();
        currencyLabel->update();
        tickerField->update();
    }

    drafts::CreateStockTransactionDraft StockWidget::Fields::getDraft() const
    {
        const auto account = accountCombo->selected();

        if (!account.has_value() || account.value().id == AccountId::invalid())
            throw std::runtime_error("No account selected");

        const auto referenceAccount = referenceAccountCombo->selected();

        if (!referenceAccount.has_value() ||
            referenceAccount.value().id == AccountId::invalid())
            throw std::runtime_error("No reference account selected");

        const auto currency     = referenceAccount->currency;
        const auto refAccountId = referenceAccount->id;

        const auto unitPrice = finance::Cash(currency, priceRow->getAmount());
        const auto quantity  = Quantity{quantityRow->getAmount()};
        const auto cash      = -quantity * unitPrice;

        auto entry = drafts::TransactionEntryDraft{
            refAccountId,
            cash,
            TransactionEntryType::General
        };

        const auto fees = -finance::Cash(currency, feesRow->getAmount());

        const auto feesEntry = drafts::TransactionEntryDraft{
            refAccountId,
            fees,
            TransactionEntryType::Fees
        };

        const auto ticker = tickerField->getTicker();
        if (!ticker.has_value())
            throw std::runtime_error("No ticker selected");

        auto tradeLeg = drafts::TradeLegDraft{
            account->id,
            unitPrice,
            quantity,
            ticker.value()
        };

        return {
            timestampField->getTimestamp(),
            {entry, feesEntry},
            {tradeLeg},
            commentField->getComment()
        };
    }

    /**
     * @brief Construct a new Stock Widget:: Stock Widget object
     *
     * @param accounts A list of account drafts to populate the account
     * combo box
     * @param referenceAccounts A list of account drafts to populate the
     * reference account combo box, this will be filtered based on the
     * selected primary account to only include accounts with the same
     * currency
     * @param tickers A list of ticker symbols to populate the ticker field
     * @param parent The parent widget for this widget
     */
    StockWidget::StockWidget(
        const std::vector<drafts::AccountDraft>& accounts,
        const std::vector<drafts::AccountDraft>& referenceAccounts,
        const std::vector<std::string>&          tickers,
        QWidget*                                 parent
    )
        : Dialog(parent),
          _layout(new QFormLayout(this)),
          _addButton(makeQChild<QPushButton>("Add Transaction", this)),
          _fields(
              std::make_unique<Fields>(
                  accounts,
                  referenceAccounts,
                  tickers,
                  this
              )
          ),
          _referenceAccounts(referenceAccounts)
    {
        setLayout(_layout);

        _fields->addFieldsToLayout(_layout);

        _addButton->setEnabled(false);
        auto* buttonLayout = makeQChild<QHBoxLayout>();
        buttonLayout->addWidget(_addButton);
        _layout->addRow(buttonLayout);

        connect(
            _fields->accountCombo,
            &AccountCombo::accountSelected,
            this,
            &StockWidget::_onAccountSelected
        );
        connect(
            _fields->referenceAccountCombo,
            &AccountCombo::accountSelected,
            this,
            &StockWidget::_onReferenceAccountSelected
        );
        _connectAddButton();
        connect(
            _fields->tickerField,
            &TickerField::createTickerRequested,
            this,
            &StockWidget::createTickerRequested
        );

        connect(_addButton, &QPushButton::clicked, this, &StockWidget::_emitOk);
    }

    StockWidget::~StockWidget() = default;

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

        _fields->priceRow->setNDecimalPlaces(getMicroUnit(account.currency));
        _fields->currencyLabel->setText(getSymbol(account.currency).c_str());

        std::vector<drafts::AccountDraft> referenceAccounts;

        for (const auto& referenceAccount : _referenceAccounts)
        {
            if (referenceAccount.currency == account.currency)
                referenceAccounts.push_back(referenceAccount);
        }

        _fields->referenceAccountCombo->updateAccounts(referenceAccounts);
        _fields->referenceAccountCombo->setEnabled(true);
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
        const auto isValid = _fields->isValid();

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
        _fields->accountCombo->updateAccounts(std::move(accounts));
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

        const auto account = _fields->accountCombo->selected();
        if (account.has_value())
            _onAccountSelected(account.value());
    }

    /**
     * @brief Update the list of tickers in the ticker field
     *
     * @param tickers The new list of ticker symbols to populate the ticker
     * field
     */
    void StockWidget::updateTickers(const std::vector<std::string>& tickers)
    {
        std::vector<QString> qTickers;
        qTickers.reserve(tickers.size());
        for (const auto& ticker : tickers)
            qTickers.emplace_back(QString::fromStdString(ticker));

        _fields->tickerField->updateTickers(std::move(qTickers));
    }

    /**
     * @brief Refresh the widget to reflect the current state
     *
     */
    void StockWidget::refresh() { _fields->update(); }

    /**
     * @brief Gets the draft for the stock transaction.
     *
     * @return drafts::CreateStockTransactionDraft The draft for the stock
     * transaction.
     */
    drafts::CreateStockTransactionDraft StockWidget::_getDraft() const
    {
        return _fields->getDraft();
    }

    /**
     * @brief Connects the add button to the appropriate signals.
     *
     */
    void StockWidget::_connectAddButton()
    {
        connect(
            _fields->priceRow,
            &AmountRow::validityChanged,
            this,
            &StockWidget::_updateAddButton
        );
        connect(
            _fields->priceRow,
            &AmountRow::valueChanged,
            this,
            &StockWidget::_updateAddButton
        );
        connect(
            _fields->tickerField,
            &TickerField::tickerSelected,
            this,
            &StockWidget::_updateAddButton
        );
    }

}   // namespace ui