#include "ui/transaction/create_transaction_dlg.hpp"

#include <qvariant.h>

#include "config/finance.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{
    CreateTransactionWidget::CreateTransactionWidget(QWidget* parent)
        : QWidget(parent)
    {
    }

    CreateTransactionDialog::CreateTransactionDialog(
        QWidget*                          parent,
        std::vector<drafts::AccountDraft> accounts
    )
        : Dialog(parent), _accounts(std::move(accounts))
    {
        _buildUI(std::nullopt);
    }

    CreateTransactionDialog::CreateTransactionDialog(
        QWidget*                          parent,
        std::vector<drafts::AccountDraft> accounts,
        TransactionType                   type
    )
        : Dialog(parent),
          _accounts(std::move(accounts)),
          _createTransactionWidget(new EmptyTransactionWidget(this))
    {
        _buildUI(type);
    }

    void CreateTransactionDialog::_buildUI(std::optional<TransactionType> type)
    {
        _transactionType = utils::makeQChild<QComboBox>(this);
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

            if (type && type.value() == enumType)
            {
                _transactionType->setCurrentIndex(
                    _transactionType->count() - 1
                );
            }
        }

        _onTransactionTypeChanged(_transactionType->currentIndex());

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
            _createTransactionWidget =
                utils::makeQChild<CreateTransactionWidget>(this);
            setWindowTitle("New Transaction");
            return;
        }

        const auto type =
            _transactionType->itemData(index).value<TransactionType>();

        setWindowTitle(
            "New " + QString::fromStdString(TransactionTypeMeta::toString(type))
        );

        switch (type)
        {
            case TransactionType::Deposit:
            case TransactionType::Withdrawal:
                _createTransactionWidget =
                    utils::makeQChild<DepositWithdrawalWidget>(this);
                break;
        }
    }
}   // namespace ui