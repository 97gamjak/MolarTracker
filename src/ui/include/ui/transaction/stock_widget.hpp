// stock_widget.hpp
#ifndef __UI__INCLUDE__UI__TRANSACTION__STOCK_WIDGET_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__STOCK_WIDGET_HPP__

#include <qwidget.h>

#include <vector>

#include "config/finance.hpp"
#include "drafts/account_draft.hpp"
#include "ui/transaction/i_create_transaction_widget.hpp"
#include "ui/transaction/ticker_field.hpp"

class QFormLayout;   // Forward declaration
class QLabel;        // Forward declaration
class QPushButton;   // Forward declaration

namespace ui
{

    class AccountCombo;   // Forward declaration
    class AmountRow;      // Forward declaration

    /**
     * @brief Widget for creating a stock transaction
     *
     * Extends the basic transaction form with a reference account selector,
     * which is kept disabled until a primary account has been chosen. The
     * add button requires both accounts and a non-zero valid amount.
     */
    class StockWidget : public ICreateTransactionWidget
    {
        Q_OBJECT
       private:
        TransactionType _type;
        QFormLayout*    _layout;
        AccountCombo*   _accountCombo;
        AccountCombo*   _referenceAccountCombo;
        AmountRow*      _quantityRow;
        AmountRow*      _priceRow;
        QLabel*         _currencyLabel;
        QPushButton*    _addButton;
        TickerField*    _tickerField;

        std::vector<drafts::AccountDraft> _referenceAccounts;

       public:
        explicit StockWidget(
            TransactionType                          type,
            std::vector<drafts::AccountDraft>        accounts,
            const std::vector<drafts::AccountDraft>& referenceAccounts,
            QWidget*                                 parent = nullptr
        );

        [[nodiscard]] TransactionType getTransactionType() const override;

       private:
        void _onAccountSelected(const drafts::AccountDraft& account);
        void _onReferenceAccountSelected(const drafts::AccountDraft& account);
        void _updateAddButton();
        void _emitOk();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__STOCK_WIDGET_HPP__