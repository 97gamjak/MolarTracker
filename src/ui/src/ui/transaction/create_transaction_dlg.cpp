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

    /**
     * @brief Construct a new Create Transaction Dialog:: Create Transaction
     * Dialog object
     *
     * @param parent
     */
    CreateTransactionDialog::CreateTransactionDialog(QWidget* parent)
        : Dialog(parent)
    {
        _buildUI();
    }

    /**
     * @brief Build the UI for the create transaction dialog, this will set up
     * the necessary widgets and layout for the dialog, including the
     * transaction type selector and the stacked widget for displaying the
     * different transaction detail forms based on the selected transaction
     * type.
     *
     */
    void CreateTransactionDialog::_buildUI()
    {
        _stack = makeQChild<QStackedWidget>(this);
        _stack->addWidget(makeQChild<EmptyTransactionWidget>(_stack));
        setWindowTitle("New Transaction");

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

        auto* layout = makeQChild<QVBoxLayout>(this);
        setLayout(layout);
        layout->addWidget(_transactionType);
        layout->addWidget(_stack);
    }

    /**
     * @brief reset the dialog to its initial state, this will clear any
     * existing transaction detail widgets from the stacked widget, reset the
     * transaction type selector to its default state, and clear the widget map,
     * allowing the dialog to be reused for creating multiple transactions
     * without needing to create a new instance of the dialog each time.
     *
     */
    void CreateTransactionDialog::reset()
    {
        _widgetMap.clear();
        _transactionType->setCurrentIndex(-1);
        for (int i = 1; i < _stack->count(); ++i)
        {
            auto* widget = _stack->widget(i);
            _stack->removeWidget(widget);
            widget->deleteLater();
        }
    }

    /**
     * @brief Handle the change of the transaction type, this will be called
     * when the user selects a different transaction type from the combo box,
     * and should handle updating the UI to display the appropriate transaction
     * detail form based on the selected transaction type, allowing the user to
     * fill out the necessary information for creating a transaction of that
     * type.
     *
     * @param index The index of the selected transaction type in the combo box,
     * this can be used to retrieve the corresponding TransactionType enum value
     * from the item data of the combo box, which can then be used to determine
     * which transaction detail form to display in the stacked widget.
     */
    void CreateTransactionDialog::_onTransactionTypeChanged(int index)
    {
        LOG_ENTRY;

        const auto type =
            _transactionType->itemData(index).value<TransactionType>();

        emit transactionTypeChanged(type);
    }

    /**
     * @brief Set the widget for the specified transaction type, this will
     * update the stacked widget to display the appropriate transaction detail
     * form based on the provided transaction type, allowing the user to fill
     * out the necessary information for creating a transaction of that type.
     *
     * @param type The transaction type for which to set the widget, this is
     * used to determine which transaction detail form to display in the stacked
     * widget, and should correspond to one of the TransactionType enum values.
     * @param accounts A vector of AccountDrafts representing the accounts that
     * can be selected in the transaction detail form, this is used to populate
     * any account selection widgets in the transaction detail form, allowing
     * the user to select from their existing accounts when creating a new
     * transaction.
     */
    void CreateTransactionDialog::setWidget(
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
    )
    {
        setWidget(type, std::move(accounts), {});
    }

    /**
     * @brief Set the widget for the specified transaction type, this will
     * update the stacked widget to display the appropriate transaction
     * detail form based on the provided transaction type, allowing the user
     * to fill out the necessary information for creating a transaction of
     * that type.
     *
     * @param type The transaction type for which to set the widget, this is
     * used to determine which transaction detail form to display in the
     * stacked widget, and should correspond to one of the TransactionType
     * enum values.
     * @param accounts A vector of AccountDrafts representing the accounts
     * that can be selected in the transaction detail form, this is used to
     * populate any account selection widgets in the transaction detail
     * form, allowing the user to select from their existing accounts when
     * creating a new transaction.
     * @param referenceAccounts A vector of AccountDrafts representing the
     * reference accounts that can be selected in the transaction detail form,
     * this is used to populate any reference account selection widgets in the
     * transaction detail form, allowing the user to select from their existing
     * reference accounts when creating a new transaction.
     */
    void CreateTransactionDialog::setWidget(
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts,
        std::vector<drafts::AccountDraft> referenceAccounts
    )
    {
        LOG_ENTRY;

        _setWindowTitle(type);

        _transactionType->setCurrentIndex(
            _transactionType->findData(QVariant::fromValue(type))
        );

        if (!_widgetMap.contains(type))
        {
            auto* widget = makeTransactionWidget(
                this,
                type,
                std::move(accounts),
                std::move(referenceAccounts)
            );

            _widgetMap[type] = widget;
            _stack->addWidget(widget);

            connect(
                widget,
                &ICreateTransactionWidget::createTransactionRequested,
                this,
                &CreateTransactionDialog::createTransactionRequested
            );
        }

        _stack->setCurrentWidget(_widgetMap[type]);
    }

    /**
     * @brief Set the window title based on the transaction type, this will
     * update the title of the dialog to reflect the selected transaction type,
     * providing context to the user about the type of transaction they are
     * creating.
     *
     * @param type The transaction type for which to set the window title, this
     * is used to determine the appropriate title to set for the dialog, and
     * should correspond to one of the TransactionType enum values.
     */
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
            case TransactionType::Stock:
                setWindowTitle("New Stock Transaction");
                return;
        }

        throw std::logic_error(
            "Unhandled transaction type: " + TransactionTypeMeta::toString(type)
        );
    }
}   // namespace ui