// stock_widget.hpp
#ifndef __UI__INCLUDE__UI__TRANSACTION__STOCK_WIDGET_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__STOCK_WIDGET_HPP__

#include <qwidget.h>

#include <vector>

#include "drafts/account/account_draft.hpp"
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

        /// The button for adding the transaction
        QPushButton* _addButton;

        struct Fields;
        /// Pointer to the fields struct
        std::unique_ptr<Fields> _fields;

        /// The list of reference accounts
        std::vector<drafts::AccountDraft> _referenceAccounts;

       public:
        explicit StockWidget(
            const std::vector<drafts::AccountDraft>& accounts,
            const std::vector<drafts::AccountDraft>& referenceAccounts,
            const std::vector<std::string>&          tickers,
            QWidget*                                 parent = nullptr
        );

        ~StockWidget() override;

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

        /**
         * @brief Emitted when a new stock transaction is requested
         *
         * @param draft The draft of the stock transaction to create
         */
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