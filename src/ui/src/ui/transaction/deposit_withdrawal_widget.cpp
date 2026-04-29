// deposit_withdrawal_widget.cpp
#include "ui/transaction/deposit_withdrawal_widget.hpp"

#include <qboxlayout.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include <stdexcept>

#include "finance/currency.hpp"
#include "ui/transaction/account_combo.hpp"
#include "ui/transaction/amount_row.hpp"
#include "utils/qt_helpers.hpp"
#include "utils/timestamp.hpp"

using utils::makeQChild;

namespace ui
{

    DepositWithdrawalWidget::DepositWithdrawalWidget(
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts,
        QWidget*                          parent
    )
        : ICreateTransactionWidget(parent),
          _type(type),
          _layout(new QFormLayout(this)),
          _accountCombo(makeQChild<AccountCombo>(std::move(accounts), this)),
          _amountRow(makeQChild<AmountRow>(this)),
          _currencyLabel(makeQChild<QLabel>(this)),
          _addButton(makeQChild<QPushButton>("Add Transaction", this))
    {
        setLayout(_layout);

        _layout->addRow("Account:", _accountCombo);

        auto* amountRowLayout = makeQChild<QHBoxLayout>();
        amountRowLayout->addWidget(_amountRow);
        amountRowLayout->addWidget(_currencyLabel);
        _layout->addRow("Amount:", amountRowLayout);

        _addButton->setEnabled(false);
        auto* buttonLayout = makeQChild<QHBoxLayout>();
        buttonLayout->addWidget(_addButton);
        _layout->addRow(buttonLayout);

        connect(
            _accountCombo,
            &AccountCombo::accountSelected,
            this,
            &DepositWithdrawalWidget::_onAccountSelected
        );
        connect(
            _amountRow,
            &AmountRow::validityChanged,
            this,
            &DepositWithdrawalWidget::_updateAddButton
        );
        connect(
            _amountRow,
            &AmountRow::valueChanged,
            this,
            &DepositWithdrawalWidget::_updateAddButton
        );
        connect(
            _addButton,
            &QPushButton::clicked,
            this,
            &DepositWithdrawalWidget::_emitOk
        );
    }

    TransactionType DepositWithdrawalWidget::getTransactionType() const
    {
        return _type;
    }

    drafts::CreateCashTransactionDraft DepositWithdrawalWidget::getDraft() const
    {
        const auto account = _accountCombo->selected();

        if (!account.has_value())
            throw std::runtime_error("No account selected");

        auto cash = finance::Cash(account->currency, _amountRow->getAmount());
        cash      = _type == TransactionType::Deposit ? cash : -cash;

        auto entry = drafts::TransactionEntryDraft{account->id, cash};
        entry.setNeedsExternal(true);

        return {Timestamp{}, {entry}, std::nullopt};
    }

    void DepositWithdrawalWidget::_onAccountSelected(
        const drafts::AccountDraft& account
    )
    {
        using finance::getMicroUnit;
        using finance::getSymbol;

        _amountRow->setNDecimalPlaces(getMicroUnit(account.currency));
        _currencyLabel->setText(getSymbol(account.currency).c_str());
        _updateAddButton();
    }

    void DepositWithdrawalWidget::_updateAddButton()
    {
        const auto isValid = _accountCombo->selected().has_value() &&
                             _amountRow->isValid() &&
                             _amountRow->getAmount() != 0;

        _addButton->setEnabled(isValid);
    }

    void DepositWithdrawalWidget::_emitOk()
    {
        emit createTransactionRequested(_type);
    }

}   // namespace ui