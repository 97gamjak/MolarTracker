// stock_widget.hpp
#ifndef __UI__INCLUDE__UI__TRANSACTION__STOCK_WIDGET_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__STOCK_WIDGET_HPP__

#include <qwidget.h>

#include <vector>

#include "drafts/account_draft.hpp"
#include "ui/base/dialog.hpp"

class QFormLayout;   // Forward declaration
class QLabel;        // Forward declaration
class QPushButton;   // Forward declaration

namespace drafts
{
    class CreateStockTransactionDraft;   // Forward declaration
}   // namespace drafts

namespace ui
{

    class AccountCombo;     // Forward declaration
    class AmountRow;        // Forward declaration
    class TickerField;      // Forward declaration
    class TimestampField;   // Forward declaration
    class CommentField;     // Forward declaration

    /**
     * @brief Widget for creating a stock transaction
     *
     * Extends the basic transaction form with a reference account selector,
     * which is kept disabled until a primary account has been chosen. The
     * add button requires both accounts and a non-zero valid amount.
     */
    class StockWidget : public Dialog
    {
        Q_OBJECT
       private:
        /// The layout for this widget
        QFormLayout* _layout;

        /// The combo box for selecting the primary account
        AccountCombo* _accountCombo;

        /// The combo box for selecting the reference account
        AccountCombo* _referenceAccountCombo;

        /// The row for entering the quantity of the stock
        AmountRow* _quantityRow;

        /// The row for entering the price of the stock
        AmountRow* _priceRow;

        /// The label for displaying the currency of the selected account
        QLabel* _currencyLabel;

        /// The button for adding the transaction
        QPushButton* _addButton;

        /// The field for entering the stock ticker
        TickerField* _tickerField;

        /// The field for entering the timestamp of the transaction
        TimestampField* _timestampField;

        /// The field for entering a comment about the transaction
        CommentField* _commentField;

        /// The list of reference accounts
        std::vector<drafts::AccountDraft> _referenceAccounts;

       public:
        explicit StockWidget(
            std::vector<drafts::AccountDraft>        accounts,
            const std::vector<drafts::AccountDraft>& referenceAccounts,
            std::vector<std::string>                 tickers,
            QWidget*                                 parent = nullptr
        );

        void updateAccounts(std::vector<drafts::AccountDraft> accounts);
        void updateReferenceAccounts(
            std::vector<drafts::AccountDraft> referenceAccounts
        );
        void updateTickers(const std::vector<std::string>& tickers);
        void refresh();

       signals:
        /**
         * @brief Emitted when a new ticker is requested
         *
         * @param ticker The ticker symbol to create
         */
        void createTickerRequested(const std::string& ticker);

        void createStockTransactionRequested(
            const drafts::CreateStockTransactionDraft& draft
        );

       private:
        void _onAccountSelected(const drafts::AccountDraft& account);
        void _onReferenceAccountSelected(const drafts::AccountDraft& account);
        void _updateAddButton();
        void _emitOk();

        [[nodiscard]]
        drafts::CreateStockTransactionDraft _getDraft() const;

        void _connectAddButton();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__STOCK_WIDGET_HPP__