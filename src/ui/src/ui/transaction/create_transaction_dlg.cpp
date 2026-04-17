#include "ui/transaction/create_transaction_dlg.hpp"

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
        _buildUI();
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
        if (index < 0)
        {
            _detailsWidget = makeQChild<CreateTransactionWidget>(this);
            _setWindowTitle(std::nullopt);
            return;
        }

        const auto type =
            _transactionType->itemData(index).value<TransactionType>();

        _setWindowTitle(type);

        emit transactionTypeChanged(type);
    }

    void CreateTransactionDialog::setWidget(
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
    )
    {
        if (_detailsWidget == nullptr ||
            !_detailsWidget->getTransactionType().has_value() ||
            _detailsWidget->getTransactionType().value() != type)
        {
            _detailsWidget = makeTransactionWidget(this, type);
            _detailsWidget->setAccounts(std::move(accounts));
        }

        setWindowTitle(_detailsWidget->windowTitle());
    }

    void CreateTransactionDialog::setTransactionType(
        std::optional<TransactionType> type
    )
    {
        int index = -1;

        if (type.has_value())
        {
            index =
                _transactionType->findData(QVariant::fromValue(type.value()));

            if (index == -1)
            {
                LOG_WARNING(
                    "Invalid transaction type set in "
                    "CreateTransactionDialog: " +
                    TransactionTypeMeta::toString(type.value())
                );
            }
        }
        _transactionType->setCurrentIndex(index);
        _onTransactionTypeChanged(index);
    }

    void CreateTransactionDialog::_setWindowTitle(
        std::optional<TransactionType> type
    )
    {
        if (!type.has_value())
        {
            setWindowTitle("New Transaction");
            return;
        }

        switch (type.value())
        {
            case TransactionType::Deposit:
                setWindowTitle("New Deposit");
                return;
            case TransactionType::Withdrawal:
                setWindowTitle("New Withdrawal");
                return;
        }

        throw std::logic_error(
            "Unhandled transaction type: " +
            std::to_string(static_cast<int>(type.value()))
        );
    }
}   // namespace ui