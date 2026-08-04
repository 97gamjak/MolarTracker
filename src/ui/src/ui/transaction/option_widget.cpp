#include "ui/transaction/option_widget.hpp"

#include <qboxlayout.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qwidget.h>

#include <QComboBox>
#include <QPointer>

#include "common/cash.hpp"
#include "common/currency.hpp"
#include "common/qt_helpers.hpp"
#include "drafts/account_draft.hpp"
#include "drafts/transaction/transaction_create_draft.hpp"
#include "ui/transaction/amount_row.hpp"
#include "ui/transaction/comment_field.hpp"
#include "ui/transaction/ticker_field.hpp"
#include "ui/transaction/timestamp_field.hpp"
#include "ui/utils/account_combo.hpp"
#include "ui/utils/error.hpp"

using common::makeQChild;

namespace ui
{

    /**
     * @brief Struct holding the fields for the option widget
     *
     */
    struct OptionWidget::Fields
    {
        /// The combo box for selecting the primary account
        QPointer<AccountCombo> accountCombo = nullptr;
        /// The combo box for selecting the reference account
        QPointer<AccountCombo> referenceAccountCombo = nullptr;
        /// The combo box for selecting the option type (call or put)
        QPointer<QComboBox> optionTypeCombo = nullptr;
        /// The combo box for selecting the action (open, close, roll)
        QPointer<QComboBox> buySellCombo = nullptr;

        /// The row for entering the quantity of the option
        QPointer<AmountRow> quantityRow = nullptr;
        /// The row for entering the price of the option
        QPointer<AmountRow> strikeRow = nullptr;
        /// The row for entering the price of the option
        QPointer<AmountRow> amountRow = nullptr;
        /// The row for entering the fees of the option
        QPointer<AmountRow> feesRow = nullptr;
        /// The row for entering the contract size of the option
        QPointer<AmountRow> contractSizeRow = nullptr;

        /// The field for entering the option ticker
        QPointer<TickerField> tickerField = nullptr;
        /// The field for entering the expiration timestamp of the option
        QPointer<TimestampField> expirationField = nullptr;
        /// The field for entering the timestamp of the transaction
        QPointer<TimestampField> timestampField = nullptr;
        /// The field for entering a comment about the transaction
        QPointer<CommentField> commentField = nullptr;

        /// The label for displaying the currency of the selected account
        QPointer<QLabel> currencyAmountLabel = nullptr;
        /// The label for displaying the currency of the strike price
        QPointer<QLabel> currencyStrikeLabel = nullptr;
        /// The label for displaying the currency of the fees
        QPointer<QLabel> currencyFeesLabel = nullptr;

        Fields(
            const std::vector<drafts::AccountDraft>& accounts,
            const std::vector<drafts::AccountDraft>& referenceAccounts,
            const Set<std::string>&                  tickers,
            QWidget*                                 parent
        );

        void addFieldsToLayout(QFormLayout* layout) const;

        [[nodiscard]]
        bool isValid() const;

        void update() const;

        [[nodiscard]]
        drafts::CreateOptionTransactionDraft getDraft() const;
    };

    /**
     * @brief Construct a new Option Widget:: Fields:: Fields object
     *
     * @param accounts
     * @param referenceAccounts
     * @param tickers
     * @param parent
     */
    OptionWidget::Fields::Fields(
        const std::vector<drafts::AccountDraft>& accounts,
        const std::vector<drafts::AccountDraft>& referenceAccounts,
        const Set<std::string>&                  tickers,
        QWidget*                                 parent
    )
        : accountCombo(new AccountCombo(accounts, parent)),
          referenceAccountCombo(new AccountCombo(referenceAccounts, parent)),
          optionTypeCombo(new QComboBox(parent)),
          buySellCombo(new QComboBox(parent)),
          quantityRow(new AmountRow(parent)),
          strikeRow(new AmountRow(parent)),
          amountRow(new AmountRow(parent)),
          feesRow(new AmountRow(parent)),
          contractSizeRow(new AmountRow(parent)),
          tickerField(new TickerField(tickers, parent)),
          expirationField(new TimestampField(true, parent)),
          timestampField(new TimestampField(false, parent)),
          commentField(new CommentField(parent)),
          currencyAmountLabel(new QLabel(parent)),
          currencyStrikeLabel(new QLabel(parent)),
          currencyFeesLabel(new QLabel(parent))
    {
        feesRow->setDefaultValue(0);
        constexpr auto defaultContractSize = 100;
        contractSizeRow->setDefaultValue(defaultContractSize);
        contractSizeRow->setNDecimalPlaces(0);

        for (const auto& optionType : OptionTypeMeta::values)
            optionTypeCombo->addItem(
                OptionTypeMeta::toString(optionType).c_str()
            );

        for (const auto& buySell : OptionBuySellMeta::values)
            buySellCombo->addItem(OptionBuySellMeta::toString(buySell).c_str());
    }

    /**
     * @brief Add the fields to the layout
     *
     * @param layout The layout to add the fields to
     */
    void OptionWidget::Fields::addFieldsToLayout(QFormLayout* layout) const
    {
        layout->addRow("Account:", accountCombo);
        layout->addRow("Reference Account:", referenceAccountCombo);
        layout->addRow("Underlying:", tickerField);
        layout->addRow("Option Type:", optionTypeCombo);
        layout->addRow("Buy/Sell:", buySellCombo);
        layout->addRow("Expiration:", expirationField);
        layout->addRow("Timestamp:", timestampField);

        auto* quantityRowLayout = makeQChild<QHBoxLayout>();
        quantityRowLayout->addWidget(quantityRow);
        layout->addRow("Quantity:", quantityRowLayout);
        quantityRow->setNDecimalPlaces(Quantity::precision);

        auto* strikeRowLayout = makeQChild<QHBoxLayout>();
        strikeRowLayout->addWidget(strikeRow);
        strikeRowLayout->addWidget(currencyStrikeLabel);
        layout->addRow("Strike Price:", strikeRowLayout);

        auto* amountRowLayout = makeQChild<QHBoxLayout>();
        amountRowLayout->addWidget(amountRow);
        amountRowLayout->addWidget(currencyAmountLabel);
        layout->addRow("Premium:", amountRowLayout);

        auto* feesRowLayout = makeQChild<QHBoxLayout>();
        feesRowLayout->addWidget(feesRow);
        feesRowLayout->addWidget(currencyFeesLabel);
        layout->addRow("Fees:", feesRowLayout);

        auto* contractSizeRowLayout = makeQChild<QHBoxLayout>();
        contractSizeRowLayout->addWidget(contractSizeRow);
        layout->addRow("Contract Size:", contractSizeRowLayout);

        layout->addRow("Comment:", commentField);

        referenceAccountCombo->setEnabled(false);
    }

    /**
     * @brief Check if the fields are valid
     *
     * @return true if all fields are valid, false otherwise
     */
    bool OptionWidget::Fields::isValid() const
    {
        if (!accountCombo->selected().has_value())
            return false;
        if (!referenceAccountCombo->selected().has_value())
            return false;

        if (!tickerField->isValid())
            return false;
        if (!quantityRow->isValid())
            return false;
        if (!strikeRow->isValid())
            return false;
        if (!amountRow->isValid())
            return false;
        if (!contractSizeRow->isValid())
            return false;

        if (quantityRow->isZero())
            return false;
        if (strikeRow->isZero())
            return false;
        if (amountRow->isZero())
            return false;
        if (contractSizeRow->isZero())
            return false;

        return true;
    }

    /**
     * @brief Update the fields
     *
     */
    void OptionWidget::Fields::update() const
    {
        accountCombo->update();
        referenceAccountCombo->update();
        quantityRow->update();
        strikeRow->update();
        amountRow->update();
        feesRow->update();
        contractSizeRow->update();
        tickerField->update();

        currencyStrikeLabel->update();
        currencyAmountLabel->update();
        currencyFeesLabel->update();
    }

    /**
     * @brief Get the draft for the option transaction
     *
     * @return drafts::CreateOptionTransactionDraft
     */
    drafts::CreateOptionTransactionDraft OptionWidget::Fields::getDraft() const
    {
        const auto account = accountCombo->selected();

        if (!account.has_value() ||
            account.value().getId() == AccountId::invalid())
            throw std::runtime_error("No account selected");

        const auto referenceAccount = referenceAccountCombo->selected();

        if (!referenceAccount.has_value() ||
            referenceAccount.value().getId() == AccountId::invalid())
            throw std::runtime_error("No reference account selected");

        const auto currency     = referenceAccount->getCurrency();
        const auto refAccountId = referenceAccount->getId();
        const auto microUnits   = getMicroUnit(currency);

        const auto strikePrice_ = strikeRow->getAmount(microUnits);
        const auto strikePrice  = Cash(currency, strikePrice_);
        const auto amount_      = amountRow->getAmount(microUnits);
        const auto amount       = Cash(currency, amount_);
        const auto quantity_    = quantityRow->getAmount(Quantity::precision);
        const auto quantity     = Quantity{quantity_};

        const auto fees_ = feesRow->getAmount(microUnits);
        const auto fees  = -Cash(currency, fees_);

        const auto ticker = tickerField->getTicker();
        if (!ticker.has_value())
            throw std::runtime_error("No ticker selected");

        return drafts::CreateOptionTransactionDraft{
            timestampField->getTimestamp(),
            ticker.value(),
            expirationField->getTimestamp(),
            static_cast<OptionType>(optionTypeCombo->currentIndex()),
            static_cast<OptionBuySell>(buySellCombo->currentIndex()),
            quantity,
            amount,
            strikePrice,
            fees,
            contractSizeRow->getAmount(0),
            account->getId(),
            refAccountId,
            commentField->getComment()
        };
    }

    /**
     * @brief Construct a new Option Widget:: Option Widget object
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
    OptionWidget::OptionWidget(
        const std::vector<drafts::AccountDraft>& accounts,
        const std::vector<drafts::AccountDraft>& referenceAccounts,
        const Set<std::string>&                  tickers,
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
            &OptionWidget::_onAccountSelected
        );
        connect(
            _fields->referenceAccountCombo,
            &AccountCombo::accountSelected,
            this,
            &OptionWidget::_onReferenceAccountSelected
        );
        _connectAddButton();
        connect(
            _fields->tickerField,
            &TickerField::createTickerRequested,
            this,
            &OptionWidget::createTickerRequested
        );

        connect(
            _addButton,
            &QPushButton::clicked,
            this,
            &OptionWidget::_emitOk
        );
    }

    OptionWidget::~OptionWidget() = default;

    /**
     * @brief Handle the selection of a primary account, this will be called
     * when the user selects a primary account from the account combo box, and
     * should handle updating the UI to reflect the selected account (e.g.
     * updating the currency label to show the currency of the selected
     * account), as well as filtering the reference accounts to only include
     * accounts with the same currency as the selected primary account, ensuring
     * that the user can only select valid reference accounts for the option
     * transaction.
     *
     * @param account The account draft of the selected primary account, this is
     * used to determine the currency of the selected account for updating the
     * UI and filtering the reference accounts.
     */
    void OptionWidget::_onAccountSelected(const drafts::AccountDraft& account)
    {
        const auto currency   = account.getCurrency();
        const auto microUnits = getMicroUnit(currency);

        _fields->strikeRow->setNDecimalPlaces(microUnits);
        _fields->amountRow->setNDecimalPlaces(microUnits);
        _fields->feesRow->setNDecimalPlaces(microUnits);
        _fields->currencyAmountLabel->setText(getSymbol(currency).c_str());
        _fields->currencyStrikeLabel->setText(getSymbol(currency).c_str());
        _fields->currencyFeesLabel->setText(getSymbol(currency).c_str());

        std::vector<drafts::AccountDraft> referenceAccounts;

        for (const auto& referenceAccount : _referenceAccounts)
        {
            if (referenceAccount.getCurrency() == currency)
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
     * proceed with creating the option transaction once they have selected a
     * valid reference account.
     *
     * @param account The account draft of the selected reference account, this
     * is used to determine if a valid reference account has been selected for
     * enabling the add button.
     */
    void OptionWidget::_onReferenceAccountSelected(
        const drafts::AccountDraft& /*account*/
    )
    {
        _updateAddButton();
    }

    /**
     * @brief Update the enabled state of the add button, this should check if
     * all required fields for creating an option transaction are valid and
     * filled out (e.g. a primary account is selected, a reference account is
     * selected, the quantity and price are valid and non-zero), and then enable
     * or disable the add button accordingly, ensuring that the user can only
     * proceed with creating the option transaction once all necessary
     * information has been provided and is valid.
     */
    void OptionWidget::_updateAddButton()
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
     * with the rest of the UI and trigger the creation of a new option
     * transaction once the user has entered all necessary information and is
     * ready to proceed.
     */
    void OptionWidget::_emitOk()
    {
        try
        {
            const auto draft = _getDraft();
            emit       createOptionTransactionRequested(draft);
        }
        catch (const std::exception& e)
        {
            ErrorDialog::show(QString(e.what()));
        }
    }

    /**
     * @brief Update the list of accounts in the account combo box
     *
     * @param accounts The new list of account drafts to populate the combo box
     */
    void OptionWidget::updateAccounts(
        std::vector<drafts::AccountDraft> accounts
    )
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
    void OptionWidget::updateReferenceAccounts(
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
    void OptionWidget::updateTickers(const Set<std::string>& tickers)
    {
        _fields->tickerField->updateTickers(common::toQStringSet(tickers));
    }

    /**
     * @brief Refresh the widget to reflect the current state
     *
     */
    void OptionWidget::refresh() { _fields->update(); }

    /**
     * @brief Gets the draft for the option transaction.
     *
     * @return drafts::CreateOptionTransactionDraft The draft for the option
     * transaction.
     */
    drafts::CreateOptionTransactionDraft OptionWidget::_getDraft() const
    {
        return _fields->getDraft();
    }

    /**
     * @brief Connects the add button to the appropriate signals.
     *
     */
    void OptionWidget::_connectAddButton()
    {
        connect(
            _fields->strikeRow,
            &AmountRow::validityChanged,
            this,
            &OptionWidget::_updateAddButton
        );
        connect(
            _fields->strikeRow,
            &AmountRow::valueChanged,
            this,
            &OptionWidget::_updateAddButton
        );
        connect(
            _fields->amountRow,
            &AmountRow::valueChanged,
            this,
            &OptionWidget::_updateAddButton
        );
        connect(
            _fields->contractSizeRow,
            &AmountRow::valueChanged,
            this,
            &OptionWidget::_updateAddButton
        );
        connect(
            _fields->tickerField,
            &TickerField::tickerSelected,
            this,
            &OptionWidget::_updateAddButton
        );
    }

}   // namespace ui
