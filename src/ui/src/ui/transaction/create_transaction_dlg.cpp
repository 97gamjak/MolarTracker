#include "ui/transaction/create_transaction_dlg.hpp"

#include <qboxlayout.h>
#include <qvariant.h>

#include "config/finance.hpp"
#include "logging/log_macros.hpp"
#include "ui/transaction/create_transaction_details.hpp"
#include "utils/qt_helpers.hpp"

REGISTER_LOG_CATEGORY("UI.Transaction.CreateTransactionDialog");

using utils::makeQChild;

namespace ui
{

    CreateTransactionDialog::CreateTransactionDialog(QWidget* parent)
        : Dialog(parent)
    {
        _stack = makeQChild<QStackedWidget>(this);
        _stack->addWidget(makeQChild<EmptyTransactionWidget>(_stack));
        setWindowTitle("New Transaction");

        _buildUI();

        auto* layout = makeQChild<QVBoxLayout>(this);
        setLayout(layout);
        layout->addWidget(_transactionType);
        layout->addWidget(_stack);
    }

    void CreateTransactionDialog::_buildUI()
    {
        _transactionType = makeQChild<QComboBox>(this);
        _transactionType->setPlaceholderText("Select Transaction Type");
        _transactionType->setCurrentIndex(-1);

        for (const auto& enumType : TransactionTypeMeta::values)
        {
            _transactionType->addItem(
                QString::fromStdString(TransactionTypeMeta::toString(enumType))
            );

            _transactionType->setItemData(
                _transactionType->count() - 1,
                QVariant::fromValue(enumType)
            );
        }

        connect(
            _transactionType,
            &QComboBox::activated,
            this,
            &CreateTransactionDialog::_onTransactionTypeChanged
        );
    }

    void CreateTransactionDialog::_onTransactionTypeChanged(int index)
    {
        LOG_ENTRY;

        const auto type =
            _transactionType->itemData(index).value<TransactionType>();

        emit transactionTypeChanged(type);
    }

    void CreateTransactionDialog::setWidget(
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
    )
    {
        LOG_ENTRY;

        _setWindowTitle(type);

        _transactionType->setCurrentIndex(
            _transactionType->findData(QVariant::fromValue(type))
        );

        if (!_widgetMap.contains(type))
        {
            auto* widget =
                makeTransactionWidget(this, type, std::move(accounts));
            _widgetMap[type] = widget;
            _stack->addWidget(widget);
        }

        _stack->setCurrentWidget(_widgetMap[type]);
    }

    void CreateTransactionDialog::_setWindowTitle(TransactionType type)
    {
        switch (type)
        {
            case TransactionType::Deposit:
                setWindowTitle("New Deposit");
                return;
            case TransactionType::Withdrawal:
                setWindowTitle("New Withdrawal");
                return;
        }

        throw std::logic_error(
            "Unhandled transaction type: " + TransactionTypeMeta::toString(type)
        );
    }
}   // namespace ui